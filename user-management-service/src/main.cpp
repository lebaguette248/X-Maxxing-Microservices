#include "config/AppComponent.hpp"
#include "controller/UserController.hpp"
#include "controller/AuthController.hpp"
#include "controller/HealthController.hpp"

#include "oatpp-swagger/Controller.hpp"

#include "oatpp/network/Server.hpp"

#include <iostream>

void run() {
    // Initialize application components
    AppComponent components;

    // Get router
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

    // Create controllers and add endpoints to router
    router->addController(UserController::createShared());
    router->addController(AuthController::createShared());
    router->addController(HealthController::createShared());
    router->addController(oatpp::swagger::Controller::createShared());

    // Get connection handler
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

    // Get connection provider
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

    // Create server
    oatpp::network::Server server(connectionProvider, connectionHandler);

    OATPP_LOGI("user-management-service", "Server running on port %s", 
               connectionProvider->getProperty("port").getData());
    OATPP_LOGI("user-management-service", "Swagger UI: http://localhost:8081/swagger/ui");

    // Start server
    server.run();
}

int main(int argc, const char* argv[]) {
    oatpp::base::Environment::init();

    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║           User Management Service v1.0.0                 ║\n";
    std::cout << "║                    Oat++ C++ REST API                    ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════╣\n";
    std::cout << "║  Endpoints:                                              ║\n";
    std::cout << "║    - Health:  http://localhost:8081/api/health           ║\n";
    std::cout << "║    - Auth:    http://localhost:8081/api/v1/auth          ║\n";
    std::cout << "║    - Users:   http://localhost:8081/api/v1/users         ║\n";
    std::cout << "║    - Swagger: http://localhost:8081/swagger/ui           ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";
    std::cout << "\n";

    run();

    std::cout << "\nEnvironment:\n";
    std::cout << "  - Object count: " << oatpp::base::Environment::getObjectsCount() << "\n";
    std::cout << "  - Object created: " << oatpp::base::Environment::getObjectsCreated() << "\n";

    oatpp::base::Environment::destroy();

    return 0;
}
