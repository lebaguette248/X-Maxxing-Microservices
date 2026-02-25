#ifndef HealthController_hpp
#define HealthController_hpp

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include "dto/ResponseDto.hpp"

#include <ctime>
#include <sstream>
#include <iomanip>

#include OATPP_CODEGEN_BEGIN(ApiController)

class HealthController : public oatpp::web::server::api::ApiController {
public:
    HealthController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper)
    {}

    // Health check endpoint
    ENDPOINT_INFO(healthCheck) {
        info->summary = "Health check";
        info->description = "Check service health status";
        info->addTag("Health");
        info->addResponse<Object<HealthDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/api/health", healthCheck)
    {
        auto health = HealthDto::createShared();
        health->status = "UP";
        health->service = "user-management-service";
        health->version = "1.0.0";
        
        // Add timestamp
        auto now = std::time(nullptr);
        std::stringstream ss;
        ss << std::put_time(std::gmtime(&now), "%Y-%m-%dT%H:%M:%SZ");
        health->timestamp = ss.str();
        
        return createDtoResponse(Status::CODE_200, health);
    }

    // Readiness probe
    ENDPOINT_INFO(readinessCheck) {
        info->summary = "Readiness check";
        info->description = "Check if service is ready to accept requests";
        info->addTag("Health");
        info->addResponse<Object<SuccessResponseDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/api/health/ready", readinessCheck)
    {
        auto response = SuccessResponseDto::createShared();
        response->success = true;
        response->message = "Service is ready";
        return createDtoResponse(Status::CODE_200, response);
    }

    // Liveness probe
    ENDPOINT_INFO(livenessCheck) {
        info->summary = "Liveness check";
        info->description = "Check if service is alive";
        info->addTag("Health");
        info->addResponse<Object<SuccessResponseDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/api/health/live", livenessCheck)
    {
        auto response = SuccessResponseDto::createShared();
        response->success = true;
        response->message = "Service is alive";
        return createDtoResponse(Status::CODE_200, response);
    }
};

#include OATPP_CODEGEN_END(ApiController)

#endif /* HealthController_hpp */
