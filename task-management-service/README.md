# Task Management Service

A microservice for managing tasks and goals built with Spring Boot 3 and Java 21.

## Features

- **Goal Management**: Create, update, delete, and track goals with progress
- **Task Management**: Full CRUD operations with ordering, status tracking, and due dates
- **API Documentation**: Interactive Swagger UI documentation
- **Docker Support**: Containerized deployment with docker-compose

## Tech Stack

- **Java 21** - Latest LTS version
- **Spring Boot 3.2** - Framework
- **Spring Data JPA** - Data persistence
- **MySQL 8** - Primary database
- **H2** - In-memory database for development/testing
- **Lombok** - Boilerplate reduction
- **MapStruct** - DTO mapping
- **SpringDoc OpenAPI** - API documentation
- **Docker** - Containerization

## Getting Started

### Prerequisites

- Java 21 or higher
- Maven 3.8+
- MySQL 8.0+ (or use H2 for development)
- Docker & Docker Compose (optional)

### Running Locally

1. **Clone the repository**
   ```bash
   git clone <repository-url>
   cd task-management-service
   ```

2. **Configure the database**
   
   For MySQL, update `application.yml` or set environment variables:
   ```bash
   export DB_URL=jdbc:mysql://localhost:3306/taskmanagement
   export DB_USERNAME=your_username
   export DB_PASSWORD=your_password
   ```

3. **Run with Maven**
   ```bash
   # Development mode with H2
   ./mvnw spring-boot:run -Dspring-boot.run.profiles=dev
   
   # Production mode with MySQL
   ./mvnw spring-boot:run
   ```

4. **Access the application**
   - API: http://localhost:8080
   - Swagger UI: http://localhost:8080/swagger-ui.html
   - H2 Console (dev): http://localhost:8080/h2-console

### Running with Docker

```bash
# Build and run all services
docker-compose up -d

# View logs
docker-compose logs -f task-management-service
```

## API Endpoints

### Goals
| Method | Endpoint | Description |
|--------|----------|-------------|
| POST | `/api/v1/goals` | Create goal |
| GET | `/api/v1/goals/{id}` | Get goal by ID |
| GET | `/api/v1/goals/user/{userId}` | Get user's goals |
| PUT | `/api/v1/goals/{id}` | Update goal |
| PATCH | `/api/v1/goals/{id}/status` | Update goal status |
| DELETE | `/api/v1/goals/{id}` | Delete goal |
| GET | `/api/v1/goals/user/{userId}/upcoming` | Get upcoming deadlines |
| GET | `/api/v1/goals/{id}/stats` | Get goal statistics |

### Tasks
| Method | Endpoint | Description |
|--------|----------|-------------|
| POST | `/api/v1/tasks` | Create task |
| POST | `/api/v1/tasks/bulk` | Create multiple tasks |
| GET | `/api/v1/tasks/{id}` | Get task by ID |
| GET | `/api/v1/tasks/goal/{goalId}` | Get tasks by goal |
| PUT | `/api/v1/tasks/{id}` | Update task |
| PATCH | `/api/v1/tasks/{id}/complete` | Toggle completion |
| DELETE | `/api/v1/tasks/{id}` | Delete task |
| PUT | `/api/v1/tasks/reorder` | Reorder tasks |
| GET | `/api/v1/tasks/user/{userId}/overdue` | Get overdue tasks |
| GET | `/api/v1/tasks/user/{userId}/today` | Get today's tasks |

### Health
| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/v1/health` | Health check |

## Project Structure

```
src/main/java/com/taskmanagement/
├── TaskManagementServiceApplication.java
├── config/
│   └── OpenApiConfig.java
├── controller/
│   ├── GoalController.java
│   ├── HealthController.java
│   └── TaskController.java
├── dto/
│   ├── request/
│   │   ├── GoalRequest.java
│   │   └── TaskRequest.java
│   └── response/
│       ├── ApiResponse.java
│       ├── GoalResponse.java
│       └── TaskResponse.java
├── entity/
│   ├── BaseEntity.java
│   ├── Goal.java
│   └── Task.java
├── exception/
│   ├── Exceptions.java
│   └── GlobalExceptionHandler.java
├── repository/
│   ├── GoalRepository.java
│   └── TaskRepository.java
└── service/
    ├── GoalService.java
    ├── TaskService.java
    └── impl/
        ├── GoalServiceImpl.java
        └── TaskServiceImpl.java
```

## Configuration

### Environment Variables

| Variable | Description | Default |
|----------|-------------|---------|
| `DB_URL` | Database JDBC URL | `jdbc:mysql://localhost:3306/taskmanagement` |
| `DB_USERNAME` | Database username | `root` |
| `DB_PASSWORD` | Database password | `password` |
| `SERVER_PORT` | Server port | `8080` |

### Profiles

- `dev` - Development with H2 in-memory database
- `test` - Testing configuration
- `prod` - Production settings with MySQL

## Microservices Integration

This service is designed to integrate into a microservices architecture:

### Service Registration
Compatible with service discovery (Eureka, Consul). Add the appropriate starter dependency:
```xml
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-netflix-eureka-client</artifactId>
</dependency>
```

### API Gateway
Designed to work behind an API Gateway (Spring Cloud Gateway, Kong). All endpoints are prefixed with `/api/v1/`.

## Testing

```bash
# Run all tests
./mvnw test

# Run with coverage
./mvnw test jacoco:report
```

## Building

```bash
# Build JAR
./mvnw clean package

# Build Docker image
docker build -t task-management-service:latest .
```

## License

MIT License
