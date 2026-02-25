#ifndef AuthController_hpp
#define AuthController_hpp

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include "dto/AuthDto.hpp"
#include "dto/ResponseDto.hpp"
#include "service/AuthService.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController)

class AuthController : public oatpp::web::server::api::ApiController {
private:
    OATPP_COMPONENT(std::shared_ptr<AuthService>, m_authService);

public:
    AuthController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper)
    {}

    // Login
    ENDPOINT_INFO(login) {
        info->summary = "User login";
        info->description = "Authenticate user and get access token";
        info->addTag("Authentication");
        info->addConsumes<Object<LoginDto>>("application/json");
        info->addResponse<Object<TokenDto>>(Status::CODE_200, "application/json");
        info->addResponse<Object<ErrorResponseDto>>(Status::CODE_401, "application/json");
    }
    ENDPOINT("POST", "/api/v1/auth/login", login,
             BODY_DTO(Object<LoginDto>, dto))
    {
        if (!dto->username || dto->username->empty()) {
            auto error = ErrorResponseDto::createShared();
            error->message = "Username is required";
            error->code = 400;
            return createDtoResponse(Status::CODE_400, error);
        }
        
        if (!dto->password || dto->password->empty()) {
            auto error = ErrorResponseDto::createShared();
            error->message = "Password is required";
            error->code = 400;
            return createDtoResponse(Status::CODE_400, error);
        }
        
        auto token = m_authService->login(dto);
        if (!token) {
            auto error = ErrorResponseDto::createShared();
            error->message = "Invalid credentials";
            error->code = 401;
            return createDtoResponse(Status::CODE_401, error);
        }
        
        return createDtoResponse(Status::CODE_200, token);
    }

    // Register
    ENDPOINT_INFO(registerUser) {
        info->summary = "User registration";
        info->description = "Register a new user and get access token";
        info->addTag("Authentication");
        info->addConsumes<Object<RegisterDto>>("application/json");
        info->addResponse<Object<TokenDto>>(Status::CODE_201, "application/json");
        info->addResponse<Object<ErrorResponseDto>>(Status::CODE_400, "application/json");
    }
    ENDPOINT("POST", "/api/v1/auth/register", registerUser,
             BODY_DTO(Object<RegisterDto>, dto))
    {
        // Validation
        if (!dto->username || dto->username->empty()) {
            auto error = ErrorResponseDto::createShared();
            error->message = "Username is required";
            error->code = 400;
            return createDtoResponse(Status::CODE_400, error);
        }
        
        if (!dto->email || dto->email->empty()) {
            auto error = ErrorResponseDto::createShared();
            error->message = "Email is required";
            error->code = 400;
            return createDtoResponse(Status::CODE_400, error);
        }
        
        if (!dto->password || dto->password->size() < 8) {
            auto error = ErrorResponseDto::createShared();
            error->message = "Password must be at least 8 characters";
            error->code = 400;
            return createDtoResponse(Status::CODE_400, error);
        }
        
        auto token = m_authService->registerUser(dto);
        if (!token) {
            auto error = ErrorResponseDto::createShared();
            error->message = "Username or email already exists";
            error->code = 400;
            return createDtoResponse(Status::CODE_400, error);
        }
        
        return createDtoResponse(Status::CODE_201, token);
    }

    // Validate token
    ENDPOINT_INFO(validateToken) {
        info->summary = "Validate token";
        info->description = "Check if access token is valid";
        info->addTag("Authentication");
        info->addResponse<Object<SuccessResponseDto>>(Status::CODE_200, "application/json");
        info->addResponse<Object<ErrorResponseDto>>(Status::CODE_401, "application/json");
    }
    ENDPOINT("POST", "/api/v1/auth/validate", validateToken,
             HEADER(String, authorization, "Authorization"))
    {
        oatpp::String token;
        
        // Extract token from "Bearer <token>" format
        if (authorization && authorization->size() > 7) {
            std::string authStr = authorization->c_str();
            if (authStr.substr(0, 7) == "Bearer ") {
                token = authStr.substr(7);
            }
        }
        
        if (!m_authService->validateToken(token)) {
            auto error = ErrorResponseDto::createShared();
            error->message = "Invalid or expired token";
            error->code = 401;
            return createDtoResponse(Status::CODE_401, error);
        }
        
        auto response = SuccessResponseDto::createShared();
        response->message = "Token is valid";
        return createDtoResponse(Status::CODE_200, response);
    }
};

#include OATPP_CODEGEN_END(ApiController)

#endif /* AuthController_hpp */
