#ifndef AppComponent_hpp
#define AppComponent_hpp

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/component.hpp"

#include "oatpp-swagger/Controller.hpp"
#include "oatpp-swagger/Resources.hpp"

#include "oatpp-sqlite/orm.hpp"

#include "SwaggerComponent.hpp"
#include "DatabaseComponent.hpp"

#include "service/UserService.hpp"
#include "service/AuthService.hpp"
#include "repository/UserRepository.hpp"

#include <cstdlib>

class AppComponent {
public:
    static oatpp::String getPort() {
        const char* port = std::getenv("SERVER_PORT");
        return port ? port : "8081";
    }

    SwaggerComponent swaggerComponent;
    DatabaseComponent databaseComponent;

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
        return oatpp::network::tcp::server::ConnectionProvider::createShared(
            {"0.0.0.0", static_cast<v_uint16>(std::stoi(getPort()->c_str())), oatpp::network::Address::IP_4}
        );
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
        return oatpp::web::server::HttpRouter::createShared();
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
        return oatpp::web::server::HttpConnectionHandler::createShared(router);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
        auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
        mapper->getSerializer()->getConfig()->useBeautifier = true;
        return mapper;
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<UserRepository>, userRepository)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::orm::DbClient>, dbClient);
        return std::make_shared<UserRepository>(dbClient);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<UserService>, userService)([] {
        OATPP_COMPONENT(std::shared_ptr<UserRepository>, repository);
        return std::make_shared<UserService>(repository);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<AuthService>, authService)([] {
        OATPP_COMPONENT(std::shared_ptr<UserRepository>, repository);
        return std::make_shared<AuthService>(repository);
    }());
};

#endif /* AppComponent_hpp */
