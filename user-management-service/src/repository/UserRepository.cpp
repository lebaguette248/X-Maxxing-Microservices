#include "UserRepository.hpp"

UserRepository::UserRepository(const std::shared_ptr<oatpp::orm::DbClient>& dbClient)
    : m_dbClient(dbClient) {}

oatpp::Object<User> UserRepository::create(const oatpp::Object<CreateUserDto>& dto, const oatpp::String& passwordHash) {
    auto conn = m_dbClient->getExecutor()->getConnection();
    auto realConn = std::static_pointer_cast<oatpp::sqlite::Connection>(conn.object);

    std::string sql = "INSERT INTO users (username, email, password_hash, first_name, last_name, role) VALUES (?, ?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(realConn->getHandle(), sql.c_str(), -1, &stmt, nullptr);
    
    sqlite3_bind_text(stmt, 1, dto->username->c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, dto->email->c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, passwordHash->c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, dto->firstName ? dto->firstName->c_str() : "", -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, dto->lastName ? dto->lastName->c_str() : "", -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, dto->role ? dto->role->c_str() : "USER", -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return nullptr;
    }

    v_int64 id = sqlite3_last_insert_rowid(realConn->getHandle());
    sqlite3_finalize(stmt);
    return findById(id);
}

oatpp::Object<User> UserRepository::findById(v_int64 id) {
    auto conn = m_dbClient->getExecutor()->getConnection();
    auto realConn = std::static_pointer_cast<oatpp::sqlite::Connection>(conn.object);

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(realConn->getHandle(), "SELECT * FROM users WHERE id = ?", -1, &stmt, nullptr);
    sqlite3_bind_int64(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        auto user = User::createShared();
        user->id = sqlite3_column_int64(stmt, 0);
        user->username = oatpp::String((const char*)sqlite3_column_text(stmt, 1));
        user->email = oatpp::String((const char*)sqlite3_column_text(stmt, 2));
        user->passwordHash = oatpp::String((const char*)sqlite3_column_text(stmt, 3));
        user->firstName = oatpp::String((const char*)sqlite3_column_text(stmt, 4));
        user->lastName = oatpp::String((const char*)sqlite3_column_text(stmt, 5));
        user->role = oatpp::String((const char*)sqlite3_column_text(stmt, 6));
        user->isActive = sqlite3_column_int(stmt, 7);
        user->createdAt = oatpp::String((const char*)sqlite3_column_text(stmt, 8));
        user->updatedAt = oatpp::String((const char*)sqlite3_column_text(stmt, 9));
        sqlite3_finalize(stmt);
        return user;
    }
    sqlite3_finalize(stmt);
    return nullptr;
}

oatpp::Object<User> UserRepository::findByUsername(const oatpp::String& username) {
    auto conn = m_dbClient->getExecutor()->getConnection();
    auto realConn = std::static_pointer_cast<oatpp::sqlite::Connection>(conn.object);

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(realConn->getHandle(), "SELECT * FROM users WHERE username = ?", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username->c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        auto user = User::createShared();
        user->id = sqlite3_column_int64(stmt, 0);
        user->username = oatpp::String((const char*)sqlite3_column_text(stmt, 1));
        user->email = oatpp::String((const char*)sqlite3_column_text(stmt, 2));
        user->passwordHash = oatpp::String((const char*)sqlite3_column_text(stmt, 3));
        user->firstName = oatpp::String((const char*)sqlite3_column_text(stmt, 4));
        user->lastName = oatpp::String((const char*)sqlite3_column_text(stmt, 5));
        user->role = oatpp::String((const char*)sqlite3_column_text(stmt, 6));
        user->isActive = sqlite3_column_int(stmt, 7);
        user->createdAt = oatpp::String((const char*)sqlite3_column_text(stmt, 8));
        user->updatedAt = oatpp::String((const char*)sqlite3_column_text(stmt, 9));
        sqlite3_finalize(stmt);
        return user;
    }
    sqlite3_finalize(stmt);
    return nullptr;
}

oatpp::Object<User> UserRepository::findByEmail(const oatpp::String& email) {
    auto conn = m_dbClient->getExecutor()->getConnection();
    auto realConn = std::static_pointer_cast<oatpp::sqlite::Connection>(conn.object);

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(realConn->getHandle(), "SELECT * FROM users WHERE email = ?", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, email->c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        auto user = User::createShared();
        user->id = sqlite3_column_int64(stmt, 0);
        user->username = oatpp::String((const char*)sqlite3_column_text(stmt, 1));
        user->email = oatpp::String((const char*)sqlite3_column_text(stmt, 2));
        user->passwordHash = oatpp::String((const char*)sqlite3_column_text(stmt, 3));
        user->firstName = oatpp::String((const char*)sqlite3_column_text(stmt, 4));
        user->lastName = oatpp::String((const char*)sqlite3_column_text(stmt, 5));
        user->role = oatpp::String((const char*)sqlite3_column_text(stmt, 6));
        user->isActive = sqlite3_column_int(stmt, 7);
        user->createdAt = oatpp::String((const char*)sqlite3_column_text(stmt, 8));
        user->updatedAt = oatpp::String((const char*)sqlite3_column_text(stmt, 9));
        sqlite3_finalize(stmt);
        return user;
    }
    sqlite3_finalize(stmt);
    return nullptr;
}

oatpp::Vector<oatpp::Object<User>> UserRepository::findAll(v_int32 offset, v_int32 limit) {
    auto conn = m_dbClient->getExecutor()->getConnection();
    auto realConn = std::static_pointer_cast<oatpp::sqlite::Connection>(conn.object);
    auto users = oatpp::Vector<oatpp::Object<User>>::createShared();

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(realConn->getHandle(), "SELECT * FROM users ORDER BY id LIMIT ? OFFSET ?", -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, limit);
    sqlite3_bind_int(stmt, 2, offset);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        auto user = User::createShared();
        user->id = sqlite3_column_int64(stmt, 0);
        user->username = oatpp::String((const char*)sqlite3_column_text(stmt, 1));
        user->email = oatpp::String((const char*)sqlite3_column_text(stmt, 2));
        user->passwordHash = oatpp::String((const char*)sqlite3_column_text(stmt, 3));
        user->firstName = oatpp::String((const char*)sqlite3_column_text(stmt, 4));
        user->lastName = oatpp::String((const char*)sqlite3_column_text(stmt, 5));
        user->role = oatpp::String((const char*)sqlite3_column_text(stmt, 6));
        user->isActive = sqlite3_column_int(stmt, 7);
        user->createdAt = oatpp::String((const char*)sqlite3_column_text(stmt, 8));
        user->updatedAt = oatpp::String((const char*)sqlite3_column_text(stmt, 9));
        users->push_back(user);
    }
    sqlite3_finalize(stmt);
    return users;
}

v_int64 UserRepository::count() {
    auto conn = m_dbClient->getExecutor()->getConnection();
    auto realConn = std::static_pointer_cast<oatpp::sqlite::Connection>(conn.object);

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(realConn->getHandle(), "SELECT COUNT(*) FROM users", -1, &stmt, nullptr);
    
    v_int64 cnt = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        cnt = sqlite3_column_int64(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return cnt;
}

bool UserRepository::update(v_int64 id, const oatpp::Object<UpdateUserDto>& dto) {
    auto existing = findById(id);
    if (!existing) return false;

    auto conn = m_dbClient->getExecutor()->getConnection();
    auto realConn = std::static_pointer_cast<oatpp::sqlite::Connection>(conn.object);

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(realConn->getHandle(), 
        "UPDATE users SET email=?, first_name=?, last_name=?, role=?, is_active=?, updated_at=CURRENT_TIMESTAMP WHERE id=?", 
        -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, dto->email ? dto->email->c_str() : existing->email->c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, dto->firstName ? dto->firstName->c_str() : (existing->firstName ? existing->firstName->c_str() : ""), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, dto->lastName ? dto->lastName->c_str() : (existing->lastName ? existing->lastName->c_str() : ""), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, dto->role ? dto->role->c_str() : existing->role->c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, dto->isActive != nullptr ? (*dto->isActive ? 1 : 0) : existing->isActive);
    sqlite3_bind_int64(stmt, 6, id);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool UserRepository::updatePassword(v_int64 id, const oatpp::String& passwordHash) {
    auto conn = m_dbClient->getExecutor()->getConnection();
    auto realConn = std::static_pointer_cast<oatpp::sqlite::Connection>(conn.object);

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(realConn->getHandle(), "UPDATE users SET password_hash=?, updated_at=CURRENT_TIMESTAMP WHERE id=?", -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, passwordHash->c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 2, id);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool UserRepository::deleteById(v_int64 id) {
    auto conn = m_dbClient->getExecutor()->getConnection();
    auto realConn = std::static_pointer_cast<oatpp::sqlite::Connection>(conn.object);

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(realConn->getHandle(), "DELETE FROM users WHERE id=?", -1, &stmt, nullptr);
    sqlite3_bind_int64(stmt, 1, id);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE && sqlite3_changes(realConn->getHandle()) > 0;
    sqlite3_finalize(stmt);
    return ok;
}

bool UserRepository::existsByUsername(const oatpp::String& username) {
    return findByUsername(username) != nullptr;
}

bool UserRepository::existsByEmail(const oatpp::String& email) {
    return findByEmail(email) != nullptr;
}
