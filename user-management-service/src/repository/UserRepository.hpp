#ifndef UserRepository_hpp
#define UserRepository_hpp

#include "oatpp-sqlite/orm.hpp"
#include "model/User.hpp"
#include "dto/UserDto.hpp"

class UserRepository {
private:
    std::shared_ptr<oatpp::orm::DbClient> m_dbClient;

public:
    explicit UserRepository(const std::shared_ptr<oatpp::orm::DbClient>& dbClient);

    oatpp::Object<User> create(const oatpp::Object<CreateUserDto>& dto, const oatpp::String& passwordHash);
    oatpp::Object<User> findById(v_int64 id);
    oatpp::Object<User> findByUsername(const oatpp::String& username);
    oatpp::Object<User> findByEmail(const oatpp::String& email);
    oatpp::Vector<oatpp::Object<User>> findAll(v_int32 offset, v_int32 limit);
    v_int64 count();
    bool update(v_int64 id, const oatpp::Object<UpdateUserDto>& dto);
    bool updatePassword(v_int64 id, const oatpp::String& passwordHash);
    bool deleteById(v_int64 id);
    bool existsByUsername(const oatpp::String& username);
    bool existsByEmail(const oatpp::String& email);
};

#endif /* UserRepository_hpp */
