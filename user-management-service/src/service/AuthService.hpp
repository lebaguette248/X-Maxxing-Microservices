#ifndef AuthService_hpp
#define AuthService_hpp

#include "dto/AuthDto.hpp"
#include "dto/UserDto.hpp"
#include "repository/UserRepository.hpp"
#include "oatpp/core/macro/component.hpp"

class AuthService {
private:
    std::shared_ptr<UserRepository> m_repository;
    std::string m_jwtSecret = "your-secret-key-change-in-production";
    int m_jwtExpirationSeconds = 3600;
    
    oatpp::String hashPassword(const oatpp::String& password);
    bool verifyPassword(const oatpp::String& password, const oatpp::String& hash);
    oatpp::String generateToken(const oatpp::Object<User>& user);
    oatpp::Object<UserDto> toUserDto(const oatpp::Object<User>& user);

public:
    explicit AuthService(const std::shared_ptr<UserRepository>& repository);
    
    oatpp::Object<TokenDto> login(const oatpp::Object<LoginDto>& dto);
    oatpp::Object<TokenDto> registerUser(const oatpp::Object<RegisterDto>& dto);
    bool validateToken(const oatpp::String& token);
};

#endif /* AuthService_hpp */
