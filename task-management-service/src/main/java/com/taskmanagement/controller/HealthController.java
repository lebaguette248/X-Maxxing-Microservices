package com.taskmanagement.controller;

import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;
import org.springframework.boot.info.BuildProperties;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.time.LocalDateTime;
import java.util.Optional;

@RestController
@RequestMapping("/api/v1")
@Tag(name = "Health", description = "Health check endpoints")
public class HealthController {

    private final Optional<BuildProperties> buildProperties;

    public HealthController(Optional<BuildProperties> buildProperties) {
        this.buildProperties = buildProperties;
    }

    @GetMapping("/health")
    @Operation(summary = "Health check", description = "Check if service is running")
    public ResponseEntity<HealthStatus> healthCheck() {
        return ResponseEntity.ok(
            HealthStatus.builder()
                .status("UP")
                .service("task-management-service")
                .version(buildProperties.map(BuildProperties::getVersion).orElse("1.0.0-SNAPSHOT"))
                .timestamp(LocalDateTime.now())
                .build()
        );
    }

    @GetMapping("/info")
    @Operation(summary = "Service info", description = "Get service information")
    public ResponseEntity<ServiceInfo> serviceInfo() {
        return ResponseEntity.ok(
            ServiceInfo.builder()
                .name("Task Management Service")
                .description("Microservice for managing tasks, goals, and users")
                .version(buildProperties.map(BuildProperties::getVersion).orElse("1.0.0-SNAPSHOT"))
                .buildTime(buildProperties.map(bp -> bp.getTime().toString()).orElse("N/A"))
                .build()
        );
    }

    @Data
    @Builder
    @NoArgsConstructor
    @AllArgsConstructor
    public static class HealthStatus {
        private String status;
        private String service;
        private String version;
        private LocalDateTime timestamp;
    }

    @Data
    @Builder
    @NoArgsConstructor
    @AllArgsConstructor
    public static class ServiceInfo {
        private String name;
        private String description;
        private String version;
        private String buildTime;
    }
}
