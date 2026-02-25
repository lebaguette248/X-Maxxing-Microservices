#ifndef DatabaseComponent_hpp
#define DatabaseComponent_hpp

#include "oatpp-sqlite/orm.hpp"
#include "oatpp/core/macro/component.hpp"

class DatabaseComponent {
public:
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider)([] {
        return std::make_shared<oatpp::sqlite::ConnectionProvider>("./userdb.sqlite");
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::sqlite::ConnectionPool>, dbConnectionPool)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, connectionProvider);
        return oatpp::sqlite::ConnectionPool::createShared(connectionProvider, 10, std::chrono::seconds(5));
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::orm::DbClient>, dbClient)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::sqlite::ConnectionPool>, connectionPool);
        
        class DbClient : public oatpp::orm::DbClient {
        public:
            DbClient(const std::shared_ptr<oatpp::orm::Executor>& executor)
                : oatpp::orm::DbClient(executor)
            {
                auto connection = getExecutor()->getConnection();
                auto realConn = std::static_pointer_cast<oatpp::sqlite::Connection>(connection.object);
                
                const char* sql = 
                    "CREATE TABLE IF NOT EXISTS users ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "username TEXT UNIQUE NOT NULL,"
                    "email TEXT UNIQUE NOT NULL,"
                    "password_hash TEXT NOT NULL,"
                    "first_name TEXT,"
                    "last_name TEXT,"
                    "role TEXT DEFAULT 'USER',"
                    "is_active INTEGER DEFAULT 1,"
                    "created_at TEXT DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at TEXT DEFAULT CURRENT_TIMESTAMP"
                    ");";
                
                sqlite3_exec(realConn->getHandle(), sql, nullptr, nullptr, nullptr);
                OATPP_LOGI("DatabaseComponent", "Database initialized");
            }
        };
        
        auto executor = std::make_shared<oatpp::sqlite::Executor>(connectionPool);
        return std::make_shared<DbClient>(executor);
    }());
};

#endif /* DatabaseComponent_hpp */
