#include "AuthService.hpp"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>

// Base64 encoding helper
static std::string base64Encode(const std::string& input) {
    static const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
    std::string result;
    int val = 0, valb = -6;
    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            result.push_back(chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) result.push_back(chars[((val << 8) >> (valb + 8)) & 0x3F]);
    return result;
}

AuthService::AuthService(const std::shared_ptr<UserRepository>& repository)
    : m_repository(repository)
{
    // Get JWT secret from environment
    const char* secret = std::getenv("JWT_SECRET");
    if (secret) {
        m_jwtSecret = secret;
    }
}

oatpp::String AuthService::hashPassword(const oatpp::String& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(password->c_str()), password->size(), hash);
    
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

bool AuthService::verifyPassword(const oatpp::String& password, const oatpp::String& hash) {
    return hashPassword(password) == hash;
}

oatpp::String AuthService::generateToken(const oatpp::Object<User>& user) {
    // Simple JWT-like token generation
    std::time_t now = std::time(nullptr);
    std::time_t exp = now + m_jwtExpirationSeconds;
    
    // Header
    std::string header = R"({"alg":"HS256","typ":"JWT"})";
    
    // Payload
    std::stringstream payload;
    payload << R"({"sub":")" << user->id << R"(",)";
    payload << R"("username":")" << user->username->c_str() << R"(",)";
    payload << R"("role":")" << user->role->c_str() << R"(",)";
    payload << R"("iat":)" << now << R"(,)";
    payload << R"("exp":)" << exp << R"(})";
    
    // Encode header and payload
    std::string encodedHeader = base64Encode(header);
    std::string encodedPayload = base64Encode(payload.str());
    
    // Create signature
    std::string data = encodedHeader + "." + encodedPayload;
    unsigned char hmacResult[EVP_MAX_MD_SIZE];
    unsigned int hmacLen;
    HMAC(EVP_sha256(), m_jwtSecret.c_str(), m_jwtSecret.length(),
         reinterpret_cast<const unsigned char*>(data.c_str()), data.length(),
         hmacResult, &hmacLen);
    
    std::string signature = base64Encode(std::string(reinterpret_cast<char*>(hmacResult), hmacLen));
    
    return data + "." + signature;
}

oatpp::Object<UserDto> AuthService::toUserDto(const oatpp::Object<User>& user) {
    auto dto = UserDto::createShared();
    dto->id = user->id;
    dto->username = user->username;
    dto->email = user->email;
    dto->firstName = user->firstName;
    dto->lastName = user->lastName;
    dto->role = user->role;
    dto->isActive = user->isActive == 1;
    dto->createdAt = user->createdAt;
    dto->updatedAt = user->updatedAt;
    return dto;
}

oatpp::Object<TokenDto> AuthService::login(const oatpp::Object<LoginDto>& dto) {
    // Try to find user by username or email
    auto user = m_repository->findByUsername(dto->username);
    if (!user) {
        user = m_repository->findByEmail(dto->username);
    }
    
    if (!user) {
        OATPP_LOGD("AuthService", "User not found: %s", dto->username->c_str());
        return nullptr;
    }
    
    if (user->isActive != 1) {
        OATPP_LOGD("AuthService", "User is not active: %s", dto->username->c_str());
        return nullptr;
    }
    
    if (!verifyPassword(dto->password, user->passwordHash)) {
        OATPP_LOGD("AuthService", "Invalid password for user: %s", dto->username->c_str());
        return nullptr;
    }
    
    // Generate token
    auto token = generateToken(user);
    
    auto result = TokenDto::createShared();
    result->accessToken = token;
    result->tokenType = "Bearer";
    result->expiresIn = m_jwtExpirationSeconds;
    result->user = toUserDto(user);
    
    return result;
}

oatpp::Object<TokenDto> AuthService::registerUser(const oatpp::Object<RegisterDto>& dto) {
    // Check if username exists
    if (m_repository->existsByUsername(dto->username)) {
        OATPP_LOGD("AuthService", "Username already exists: %s", dto->username->c_str());
        return nullptr;
    }
    
    // Check if email exists
    if (m_repository->existsByEmail(dto->email)) {
        OATPP_LOGD("AuthService", "Email already exists: %s", dto->email->c_str());
        return nullptr;
    }
    
    // Create user DTO
    auto createDto = CreateUserDto::createShared();
    createDto->username = dto->username;
    createDto->email = dto->email;
    createDto->password = dto->password;
    createDto->firstName = dto->firstName;
    createDto->lastName = dto->lastName;
    createDto->role = "USER";
    
    // Hash password and create user
    auto passwordHash = hashPassword(dto->password);
    auto user = m_repository->create(createDto, passwordHash);
    
    if (!user) {
        return nullptr;
    }
    
    // Generate token
    auto token = generateToken(user);
    
    auto result = TokenDto::createShared();
    result->accessToken = token;
    result->tokenType = "Bearer";
    result->expiresIn = m_jwtExpirationSeconds;
    result->user = toUserDto(user);
    
    return result;
}

bool AuthService::validateToken(const oatpp::String& token) {
    // Simple token validation - just check format
    // In production, verify signature and expiration
    if (!token || token->empty()) {
        return false;
    }
    
    std::string tokenStr = token->c_str();
    int dotCount = 0;
    for (char c : tokenStr) {
        if (c == '.') dotCount++;
    }
    
    return dotCount == 2;
}
