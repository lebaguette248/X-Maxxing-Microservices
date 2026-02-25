#ifndef AuthDto_hpp
#define AuthDto_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include "UserDto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class LoginDto : public oatpp::DTO {
    DTO_INIT(LoginDto, DTO)
    DTO_FIELD(String, username);
    DTO_FIELD(String, password);
};

class RegisterDto : public oatpp::DTO {
    DTO_INIT(RegisterDto, DTO)
    DTO_FIELD(String, username);
    DTO_FIELD(String, email);
    DTO_FIELD(String, password);
    DTO_FIELD(String, firstName, "first_name");
    DTO_FIELD(String, lastName, "last_name");
};

class TokenDto : public oatpp::DTO {
    DTO_INIT(TokenDto, DTO)
    DTO_FIELD(String, accessToken, "access_token");
    DTO_FIELD(String, tokenType, "token_type") = "Bearer";
    DTO_FIELD(Int64, expiresIn, "expires_in");
    DTO_FIELD(Object<UserDto>, user);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* AuthDto_hpp */
