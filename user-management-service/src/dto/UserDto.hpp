#ifndef UserDto_hpp
#define UserDto_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * User Response DTO (returned to client)
 */
class UserDto : public oatpp::DTO {
    DTO_INIT(UserDto, DTO)

    DTO_FIELD(Int64, id);
    DTO_FIELD(String, username);
    DTO_FIELD(String, email);
    DTO_FIELD(String, firstName, "first_name");
    DTO_FIELD(String, lastName, "last_name");
    DTO_FIELD(String, role);
    DTO_FIELD(Boolean, isActive, "is_active");
    DTO_FIELD(String, createdAt, "created_at");
    DTO_FIELD(String, updatedAt, "updated_at");
};

/**
 * Create User Request DTO
 */
class CreateUserDto : public oatpp::DTO {
    DTO_INIT(CreateUserDto, DTO)

    DTO_FIELD_INFO(username) {
        info->description = "Unique username";
        info->required = true;
    }
    DTO_FIELD(String, username);

    DTO_FIELD_INFO(email) {
        info->description = "User email address";
        info->required = true;
    }
    DTO_FIELD(String, email);

    DTO_FIELD_INFO(password) {
        info->description = "User password (min 8 characters)";
        info->required = true;
    }
    DTO_FIELD(String, password);

    DTO_FIELD_INFO(firstName) {
        info->description = "User's first name";
    }
    DTO_FIELD(String, firstName, "first_name");

    DTO_FIELD_INFO(lastName) {
        info->description = "User's last name";
    }
    DTO_FIELD(String, lastName, "last_name");

    DTO_FIELD_INFO(role) {
        info->description = "User role (USER, ADMIN, MODERATOR)";
    }
    DTO_FIELD(String, role) = "USER";
};

/**
 * Update User Request DTO
 */
class UpdateUserDto : public oatpp::DTO {
    DTO_INIT(UpdateUserDto, DTO)

    DTO_FIELD(String, email);
    DTO_FIELD(String, firstName, "first_name");
    DTO_FIELD(String, lastName, "last_name");
    DTO_FIELD(String, role);
    DTO_FIELD(Boolean, isActive, "is_active");
};

/**
 * Change Password DTO
 */
class ChangePasswordDto : public oatpp::DTO {
    DTO_INIT(ChangePasswordDto, DTO)

    DTO_FIELD_INFO(currentPassword) {
        info->description = "Current password";
        info->required = true;
    }
    DTO_FIELD(String, currentPassword, "current_password");

    DTO_FIELD_INFO(newPassword) {
        info->description = "New password (min 8 characters)";
        info->required = true;
    }
    DTO_FIELD(String, newPassword, "new_password");
};

/**
 * User List Response DTO
 */
class UserPageDto : public oatpp::DTO {
    DTO_INIT(UserPageDto, DTO)

    DTO_FIELD(Vector<Object<UserDto>>, items);
    DTO_FIELD(Int32, page);
    DTO_FIELD(Int32, size);
    DTO_FIELD(Int64, totalElements, "total_elements");
    DTO_FIELD(Int32, totalPages, "total_pages");
};

#include OATPP_CODEGEN_END(DTO)

#endif /* UserDto_hpp */
