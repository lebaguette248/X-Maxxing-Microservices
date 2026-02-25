cmake_minimum_required(VERSION 3.20)
project(user-management-service VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# ============================================================================
# Dependencies via FetchContent
# ============================================================================
include(FetchContent)

# Oat++ core
FetchContent_Declare(
    oatpp
    GIT_REPOSITORY https://github.com/oatpp/oatpp.git
    GIT_TAG 1.3.0
)

# Oat++ Swagger (OpenAPI)
FetchContent_Declare(
    oatpp-swagger
    GIT_REPOSITORY https://github.com/oatpp/oatpp-swagger.git
    GIT_TAG 1.3.0
)

# Oat++ SQLite (lightweight database)
FetchContent_Declare(
    oatpp-sqlite
    GIT_REPOSITORY https://github.com/oatpp/oatpp-sqlite.git
    GIT_TAG 1.3.0
)

FetchContent_MakeAvailable(oatpp oatpp-swagger oatpp-sqlite)

# ============================================================================
# Application sources
# ============================================================================
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/service/UserService.cpp
    src/service/AuthService.cpp
    src/repository/UserRepository.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    oatpp
    oatpp-swagger
    oatpp-sqlite
)

# ============================================================================
# Copy Swagger UI resources
# ============================================================================
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${oatpp-swagger_SOURCE_DIR}/res
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
)

# ============================================================================
# Database initialization script
# ============================================================================
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/sql/init.sql
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/sql/init.sql
)
