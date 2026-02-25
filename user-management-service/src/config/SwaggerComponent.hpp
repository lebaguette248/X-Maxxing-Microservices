#ifndef SwaggerComponent_hpp
#define SwaggerComponent_hpp

#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include "oatpp/core/macro/component.hpp"

class SwaggerComponent {
public:
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)([] {
        oatpp::swagger::DocumentInfo::Builder builder;
        
        builder
            .setTitle("User Management Service API")
            .setDescription("High-performance User Management microservice built with Oat++ (C++)")
            .setVersion("1.0.0")
            .setContactName("X-Maxxing Team")
            .setLicenseName("MIT")
            .addServer("http://localhost:8081", "Development server");

        return builder.build();
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)([] {
        return oatpp::swagger::Resources::loadResources("res");
    }());
};

#endif /* SwaggerComponent_hpp */
