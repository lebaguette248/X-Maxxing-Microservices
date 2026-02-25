# User Management Service

High-performance User Management microservice built with **Oat++** (C++ Web Framework).

## Features

- User CRUD operations
- Authentication (login/register)
- JWT token generation
- Password hashing (SHA-256)
- SQLite database (lightweight, embedded)
- Swagger UI documentation
- Docker support
- Health check endpoints

## Technology Stack

- **Framework**: Oat++ 1.3.0
- **Language**: C++17
- **Database**: SQLite
- **API Docs**: OpenAPI/Swagger
- **Build**: CMake

## Prerequisites

### Linux/macOS
```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake git libsqlite3-dev libssl-dev

# macOS (with Homebrew)
brew install cmake sqlite openssl
```

### Windows
- Visual Studio 2019+ with C++ tools
- CMake 3.20+
- vcpkg (recommended for dependencies)

## Build

```bash
# Clone/navigate to project
cd user-management-service

# Create build directory
mkdir build && cd build

# Configure
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build
cmake --build . --parallel

# Run
./user-management-service
```

## Docker

```bash
# Build image
docker build -t user-management-service .

# Run container
docker run -d -p 8081:8081 \
  -e JWT_SECRET=your-secret-key \
  --name user-service \
  user-management-service

# Or use docker-compose
docker-compose up -d
```

## API Endpoints

### Health
| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/health` | Health check |
| GET | `/api/health/ready` | Readiness probe |
| GET | `/api/health/live` | Liveness probe |

### Authentication
| Method | Endpoint | Description |
|--------|----------|-------------|
| POST | `/api/v1/auth/register` | Register new user |
| POST | `/api/v1/auth/login` | User login |
| POST | `/api/v1/auth/validate` | Validate token |

### Users
| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/v1/users` | Get all users (paginated) |
| GET | `/api/v1/users/{id}` | Get user by ID |
| POST | `/api/v1/users` | Create user |
| PUT | `/api/v1/users/{id}` | Update user |
| DELETE | `/api/v1/users/{id}` | Delete user |
| PATCH | `/api/v1/users/{id}/password` | Change password |

## Swagger UI

Access the interactive API documentation at:
```
http://localhost:8081/swagger/ui
```

## Configuration

Environment variables:
| Variable | Default | Description |
|----------|---------|-------------|
| `SERVER_PORT` | 8081 | Server port |
| `JWT_SECRET` | (hardcoded) | JWT signing secret |

## Example Requests

### Register
```bash
curl -X POST http://localhost:8081/api/v1/auth/register \
  -H "Content-Type: application/json" \
  -d '{
    "username": "johndoe",
    "email": "john@example.com",
    "password": "securepass123",
    "first_name": "John",
    "last_name": "Doe"
  }'
```

### Login
```bash
curl -X POST http://localhost:8081/api/v1/auth/login \
  -H "Content-Type: application/json" \
  -d '{
    "username": "johndoe",
    "password": "securepass123"
  }'
```

### Get Users
```bash
curl http://localhost:8081/api/v1/users?page=0&size=10
```

## Default Admin

The service creates a default admin user on startup:
- **Username**: admin
- **Password**: admin123
- **Role**: ADMIN

## Project Structure

```
user-management-service/
├── CMakeLists.txt
├── Dockerfile
├── docker-compose.yml
├── README.md
├── sql/
│   └── init.sql
└── src/
    ├── main.cpp
    ├── config/
    │   ├── AppComponent.hpp
    │   ├── DatabaseComponent.hpp
    │   └── SwaggerComponent.hpp
    ├── controller/
    │   ├── AuthController.hpp
    │   ├── HealthController.hpp
    │   └── UserController.hpp
    ├── dto/
    │   ├── AuthDto.hpp
    │   ├── ResponseDto.hpp
    │   └── UserDto.hpp
    ├── model/
    │   └── User.hpp
    ├── repository/
    │   ├── UserRepository.cpp
    │   └── UserRepository.hpp
    └── service/
        ├── AuthService.cpp
        ├── AuthService.hpp
        ├── UserService.cpp
        └── UserService.hpp
```

## Performance

Oat++ is designed for high performance:
- Zero-copy operations where possible
- Asynchronous I/O support
- Connection pooling
- Minimal memory footprint

## License

MIT License
