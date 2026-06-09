cmake_minimum_required(VERSION 3.15)
project(TCP_IP VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib)

enable_testing()

option(BUILD_TESTING "Build unit tests" ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")

# External dependency
add_subdirectory(${CMAKE_SOURCE_DIR}/ThirdParty/spdlog)

# Shared library for common implementation
set(TCP_LIB_SOURCES
    src/TcpServer.cpp
    src/ClientSession.cpp
    src/TcpClient.cpp
    Log/log.cpp
)

add_library(tcp_lib STATIC ${TCP_LIB_SOURCES})

target_include_directories(tcp_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_features(tcp_lib PUBLIC cxx_std_23)

target_compile_options(tcp_lib PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

target_link_libraries(tcp_lib PUBLIC
    spdlog::spdlog
)

add_executable(server src/main.cpp)
add_executable(client src/clientMain.cpp)

target_link_libraries(server PRIVATE tcp_lib)

target_link_libraries(client PRIVATE tcp_lib)

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

  if(BUILD_TESTING)
    add_subdirectory(${CMAKE_SOURCE_DIR}/test)
  endif()

