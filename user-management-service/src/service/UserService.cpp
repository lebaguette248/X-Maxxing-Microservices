#include "UserService.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>

std::string UserService::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%dT%H:%M:%S");
    return ss.str();
}

oatpp::Object<UserResponseDto> UserService::toUserResponse(const oatpp::Object<UserDto>& user) {
    if (!user) return nullptr;
    auto response = UserResponseDto::createShared();
    response->id = user->id;
    response->username = user->username;
    response->email = user->email;
    response->firstName = user->firstName;
    response->lastName = user->lastName;
    response->role = user->role;
    response->status = user->status;
    response->phone = user->phone;
    response->avatarUrl = user->avatarUrl;
    response->createdAt = user->createdAt;
    response->updatedAt = user->updatedAt;
    response->lastLoginAt = user->lastLoginAt;
    return response;
}

oatpp::Object<UserResponseDto> UserService::createUser(const oatpp::Object<CreateUserDto>& dto) {
    return toUserResponse(m_repository->createUser(dto));
}

oatpp::Object<UserResponseDto> UserService::getUserById(v_int64 id) {
    return toUserResponse(m_repository->getUserById(id));
}

oatpp::Object<UserResponseDto> UserService::getUserByUsername(const oatpp::String& username) {
    return toUserResponse(m_repository->getUserByUsername(username));
}

oatpp::Vector<oatpp::Object<UserResponseDto>> UserService::getAllUsers() {
    auto users = m_repository->getAllUsers();
    auto responses = oatpp::Vector<oatpp::Object<UserResponseDto>>::createShared();
    for (const auto& user : *users) responses->push_back(toUserResponse(user));
    return responses;
}

oatpp::Vector<oatpp::Object<UserResponseDto>> UserService::getUsersByStatus(const oatpp::String& status) {
    UserStatus userStatus;
    if (status == "ACTIVE") userStatus = UserStatus::ACTIVE;
    else if (status == "INACTIVE") userStatus = UserStatus::INACTIVE;
    else if (status == "SUSPENDED") userStatus = UserStatus::SUSPENDED;
    else if (status == "PENDING") userStatus = UserStatus::PENDING;
    else return oatpp::Vector<oatpp::Object<UserResponseDto>>::createShared();
    
    auto users = m_repository->getUsersByStatus(userStatus);
    auto responses = oatpp::Vector<oatpp::Object<UserResponseDto>>::createShared();
    for (const auto& user : *users) responses->push_back(toUserResponse(user));
    return responses;
}

oatpp::Vector<oatpp::Object<UserResponseDto>> UserService::getUsersByRole(const oatpp::String& role) {
    UserRole userRole;
    if (role == "ADMIN") userRole = UserRole::ADMIN;
    else if (role == "USER") userRole = UserRole::USER;
    else if (role == "MODERATOR") userRole = UserRole::MODERATOR;
    else return oatpp::Vector<oatpp::Object<UserResponseDto>>::createShared();
    
    auto users = m_repository->getUsersByRole(userRole);
    auto responses = oatpp::Vector<oatpp::Object<UserResponseDto>>::createShared();
    for (const auto& user : *users) responses->push_back(toUserResponse(user));
    return responses;
}

oatpp::Object<UserResponseDto> UserService::updateUser(v_int64 id, const oatpp::Object<UpdateUserDto>& dto) {
    return toUserResponse(m_repository->updateUser(id, dto));
}

oatpp::Object<UserResponseDto> UserService::updateUserStatus(v_int64 id, const oatpp::Object<UpdateStatusDto>& dto) {
    return toUserResponse(m_repository->updateUserStatus(id, dto->status));
}

oatpp::Object<UserResponseDto> UserService::updateUserRole(v_int64 id, const oatpp::Object<UpdateRoleDto>& dto) {
    return toUserResponse(m_repository->updateUserRole(id, dto->role));
}

bool UserService::changePassword(v_int64 id, const oatpp::Object<ChangePasswordDto>& dto) {
    auto user = m_repository->getUserById(id);
    if (!user || user->password != dto->currentPassword) return false;
    return m_repository->updatePassword(id, dto->newPassword);
}

bool UserService::deleteUser(v_int64 id) {
    return m_repository->deleteUser(id);
}

oatpp::Object<UserResponseDto> UserService::authenticate(const oatpp::Object<LoginDto>& dto) {
    auto user = m_repository->getUserByUsername(dto->username);
    if (!user || user->password != dto->password || user->status != UserStatus::ACTIVE) return nullptr;
    m_repository->updateLastLogin(user->id);
    return toUserResponse(user);
}

bool UserService::usernameExists(const oatpp::String& username) { return m_repository->usernameExists(username); }
bool UserService::emailExists(const oatpp::String& email) { return m_repository->emailExists(email); }
v_int64 UserService::getUserCount() { return m_repository->getUserCount(); }
