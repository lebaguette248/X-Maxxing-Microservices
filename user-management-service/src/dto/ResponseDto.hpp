#ifndef ResponseDto_hpp
#define ResponseDto_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Generic API Response wrapper
 */
template<class T>
class ApiResponse : public oatpp::DTO {
    DTO_INIT(ApiResponse, DTO)

    DTO_FIELD(Boolean, success) = true;
    DTO_FIELD(String, message);
    DTO_FIELD(T, data);
    DTO_FIELD(String, timestamp);
};

/**
 * Success Response DTO
 */
class SuccessResponseDto : public oatpp::DTO {
    DTO_INIT(SuccessResponseDto, DTO)

    DTO_FIELD(Boolean, success) = true;
    DTO_FIELD(String, message);
    DTO_FIELD(String, timestamp);
};

/**
 * Error Response DTO
 */
class ErrorResponseDto : public oatpp::DTO {
    DTO_INIT(ErrorResponseDto, DTO)

    DTO_FIELD(Boolean, success) = false;
    DTO_FIELD(String, message);
    DTO_FIELD(Int32, code);
    DTO_FIELD(String, timestamp);
    DTO_FIELD(Vector<String>, errors);
};

/**
 * Health Check Response DTO
 */
class HealthDto : public oatpp::DTO {
    DTO_INIT(HealthDto, DTO)

    DTO_FIELD(String, status) = "UP";
    DTO_FIELD(String, service) = "user-management-service";
    DTO_FIELD(String, version) = "1.0.0";
    DTO_FIELD(String, timestamp);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* ResponseDto_hpp */
