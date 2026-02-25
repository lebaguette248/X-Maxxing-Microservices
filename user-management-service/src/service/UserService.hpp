#ifndef UserService_hpp
#define UserService_hpp

#include "repository/UserRepository.hpp"
#include "dto/UserDto.hpp"

class UserService {
private:
    std::shared_ptr<UserRepository> m_repository;
    std::string getCurrentTimestamp();
    oatpp::Object<UserResponseDto> toUserResponse(const oatpp::Object<UserDto>& user);

public:
    UserService(const std::shared_ptr<UserRepository>& repository) : m_repository(repository) {}
    
    oatpp::Object<UserResponseDto> createUser(const oatpp::Object<CreateUserDto>& dto);
    oatpp::Object<UserResponseDto> getUserById(v_int64 id);
    oatpp::Object<UserResponseDto> getUserByUsername(const oatpp::String& username);
    oatpp::Vector<oatpp::Object<UserResponseDto>> getAllUsers();
    oatpp::Vector<oatpp::Object<UserResponseDto>> getUsersByStatus(const oatpp::String& status);
    oatpp::Vector<oatpp::Object<UserResponseDto>> getUsersByRole(const oatpp::String& role);
    oatpp::Object<UserResponseDto> updateUser(v_int64 id, const oatpp::Object<UpdateUserDto>& dto);
    oatpp::Object<UserResponseDto> updateUserStatus(v_int64 id, const oatpp::Object<UpdateStatusDto>& dto);
    oatpp::Object<UserResponseDto> updateUserRole(v_int64 id, const oatpp::Object<UpdateRoleDto>& dto);
    bool changePassword(v_int64 id, const oatpp::Object<ChangePasswordDto>& dto);
    bool deleteUser(v_int64 id);
    oatpp::Object<UserResponseDto> authenticate(const oatpp::Object<LoginDto>& dto);
    bool usernameExists(const oatpp::String& username);
    bool emailExists(const oatpp::String& email);
    v_int64 getUserCount();
};

#endif
