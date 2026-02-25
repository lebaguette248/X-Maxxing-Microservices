#ifndef UserController_hpp
#define UserController_hpp

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include "dto/UserDto.hpp"
#include "dto/ResponseDto.hpp"
#include "service/UserService.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

class UserController : public oatpp::web::server::api::ApiController {
private:
    OATPP_COMPONENT(std::shared_ptr<UserService>, m_userService);

public:
    UserController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper)
    {}

    // Get all users (paginated)
    ENDPOINT_INFO(getUsers) {
        info->summary = "Get all users";
        info->description = "Get paginated list of users";
        info->addTag("Users");
        info->addResponse<Object<UserPageDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/api/v1/users", getUsers,
             QUERY(Int32, page, "page", "0"),
             QUERY(Int32, size, "size", "10"))
    {
        auto result = m_userService->getAllUsers(page, size);
        return createDtoResponse(Status::CODE_200, result);
    }

    // Get user by ID
    ENDPOINT_INFO(getUserById) {
        info->summary = "Get user by ID";
        info->addTag("Users");
        info->pathParams["id"].description = "User ID";
        info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/api/v1/users/{id}", getUserById,
             PATH(Int64, id))
    {
        auto user = m_userService->getUserById(id);
        if (!user) {
            auto error = ErrorResponseDto::createShared();
            error->message = "User not found";
            error->code = 404;
            return createDtoResponse(Status::CODE_404, error);
        }
        return createDtoResponse(Status::CODE_200, user);
    }

    // Create user
    ENDPOINT_INFO(createUser) {
        info->summary = "Create user";
        info->addTag("Users");
        info->addConsumes<Object<CreateUserDto>>("application/json");
        info->addResponse<Object<UserDto>>(Status::CODE_201, "application/json");
    }
    ENDPOINT("POST", "/api/v1/users", createUser,
             BODY_DTO(Object<CreateUserDto>, dto))
    {
        if (!dto->username || dto->username->empty()) {
            auto error = ErrorResponseDto::createShared();
            error->message = "Username is required";
            error->code = 400;
            return createDtoResponse(Status::CODE_400, error);
        }
        
        auto user = m_userService->createUser(dto);
        if (!user) {
            auto error = ErrorResponseDto::createShared();
            error->message = "Username or email already exists";
            error->code = 400;
            return createDtoResponse(Status::CODE_400, error);
        }
        
        return createDtoResponse(Status::CODE_201, user);
    }

    // Update user
    ENDPOINT_INFO(updateUser) {
        info->summary = "Update user";
        info->addTag("Users");
        info->pathParams["id"].description = "User ID";
        info->addConsumes<Object<UpdateUserDto>>("application/json");
        info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("PUT", "/api/v1/users/{id}", updateUser,
             PATH(Int64, id),
             BODY_DTO(Object<UpdateUserDto>, dto))
    {
        auto user = m_userService->updateUser(id, dto);
        if (!user) {
            auto error = ErrorResponseDto::createShared();
            error->message = "User not found";
            error->code = 404;
            return createDtoResponse(Status::CODE_404, error);
        }
        return createDtoResponse(Status::CODE_200, user);
    }

    // Delete user
    ENDPOINT_INFO(deleteUser) {
        info->summary = "Delete user";
        info->addTag("Users");
        info->pathParams["id"].description = "User ID";
        info->addResponse<Object<SuccessResponseDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("DELETE", "/api/v1/users/{id}", deleteUser,
             PATH(Int64, id))
    {
        if (!m_userService->deleteUser(id)) {
            auto error = ErrorResponseDto::createShared();
            error->message = "User not found";
            error->code = 404;
            return createDtoResponse(Status::CODE_404, error);
        }
        
        auto response = SuccessResponseDto::createShared();
        response->message = "User deleted successfully";
        return createDtoResponse(Status::CODE_200, response);
    }

    // Change password
    ENDPOINT_INFO(changePassword) {
        info->summary = "Change password";
        info->addTag("Users");
        info->addConsumes<Object<ChangePasswordDto>>("application/json");
        info->addResponse<Object<SuccessResponseDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("PATCH", "/api/v1/users/{id}/password", changePassword,
             PATH(Int64, id),
             BODY_DTO(Object<ChangePasswordDto>, dto))
    {
        if (!m_userService->changePassword(id, dto->currentPassword, dto->newPassword)) {
            auto error = ErrorResponseDto::createShared();
            error->message = "Invalid current password or user not found";
            error->code = 400;
            return createDtoResponse(Status::CODE_400, error);
        }
        
        auto response = SuccessResponseDto::createShared();
        response->message = "Password changed successfully";
        return createDtoResponse(Status::CODE_200, response);
    }
};

#include OATPP_CODEGEN_END(ApiController)

#endif /* UserController_hpp */
