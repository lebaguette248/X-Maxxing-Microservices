package com.taskmanagement.config;

import io.swagger.v3.oas.annotations.OpenAPIDefinition;
import io.swagger.v3.oas.annotations.info.Contact;
import io.swagger.v3.oas.annotations.info.Info;
import io.swagger.v3.oas.annotations.info.License;
import io.swagger.v3.oas.annotations.servers.Server;
import org.springframework.context.annotation.Configuration;

@Configuration
@OpenAPIDefinition(
    info = @Info(
        title = "Task Management Service API",
        version = "1.0.0",
        description = "RESTful API for managing tasks and goals in a microservices environment",
        contact = @Contact(
            name = "API Support",
            email = "support@taskmanagement.com"
        ),
        license = @License(
            name = "MIT License",
            url = "https://opensource.org/licenses/MIT"
        )
    ),
    servers = {
        @Server(url = "http://localhost:8080", description = "Development server"),
        @Server(url = "https://api.taskmanagement.com", description = "Production server")
    }
)
public class OpenApiConfig {
}
