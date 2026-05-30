cmake_minimum_required(VERSION 3.15)
project(TCP_IP VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Prefer release by default for this small server project
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")

# External dependency
add_subdirectory(ThirdParty/spdlog)

# Application source files
set(SERVER_SOURCES
    src/main.cpp
    src/TcpServer.cpp
    src/ClientSession.cpp
    Log/log.cpp
)

set(CLIENT_SOURCES
    src/clientMain.cpp
    src/TcpClient.cpp
    Log/log.cpp
)

add_executable(server ${SERVER_SOURCES})
add_executable(client ${CLIENT_SOURCES})

target_include_directories(server PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_include_directories(client PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_features(server PRIVATE cxx_std_23)

target_compile_features(client PRIVATE cxx_std_23)

target_compile_options(server PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

target_compile_options(client PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

target_link_libraries(server PRIVATE
    spdlog::spdlog
)

target_link_libraries(client PRIVATE
    spdlog::spdlog
)

