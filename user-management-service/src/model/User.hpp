#ifndef User_hpp
#define User_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class User : public oatpp::DTO {
    DTO_INIT(User, DTO)
    DTO_FIELD(Int64, id);
    DTO_FIELD(String, username);
    DTO_FIELD(String, email);
    DTO_FIELD(String, passwordHash, "password_hash");
    DTO_FIELD(String, firstName, "first_name");
    DTO_FIELD(String, lastName, "last_name");
    DTO_FIELD(String, role);
    DTO_FIELD(Int32, isActive, "is_active");
    DTO_FIELD(String, createdAt, "created_at");
    DTO_FIELD(String, updatedAt, "updated_at");
};

#include OATPP_CODEGEN_END(DTO)

#endif /* User_hpp */
