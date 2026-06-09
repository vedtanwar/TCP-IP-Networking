cmake_minimum_required(VERSION 3.15)

project(TCP_IP_TESTS)

include(FetchContent)
find_package(Threads REQUIRED)

set(GTEST_VERSION "1.14.0")

# Force local folder (THIS IS IMPORTANT)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/ThirdParty)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v${GTEST_VERSION}
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/googletest
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_tests
    test_ClientSession.cpp
    test_TcpClient.cpp
    test_TcpServer.cpp
)

target_include_directories(unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(unit_tests PRIVATE
    tcp_lib
    GTest::gtest_main
    Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(unit_tests)