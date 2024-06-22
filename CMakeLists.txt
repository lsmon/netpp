cmake_minimum_required(VERSION 3.11)
project(netpp)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION_TWEAK 0)

set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}")

message(STATUS "INFO -- CONFIGURE START")
include(${CMAKE_CURRENT_SOURCE_DIR}/external/CMakeLists.txt)
message(STATUS "INFO -- CONFIGURE FINISHED")

message(STATUS "INFO -- JSON_LIB:   ${JSON_LIB}")
message(STATUS "INFO -- JSON_INC:   ${JSON_INC}")
message(STATUS "INFO -- LOGPP_LIB:  ${LOGPP_LIB}")
message(STATUS "INFO -- LOGPP_INC:  ${LOGPP_INC}")

find_package(OpenSSL REQUIRED)

message(STATUS "OPENSSL_INCLUDE_DIR:    ${OPENSSL_INCLUDE_DIR}")
include_directories(${OPENSSL_INCLUDE_DIR})

include(http-server.cmake)

message(STATUS "INFO -- _${LIB_NAME_SERVER}")

include(http-client.cmake)

enable_testing()

include(test/CMakeLists.txt)

# add_test(TEST_SERVER ${PROJECT_NAME}_test server)
add_test(TEST_CLIENT ${PROJECT_NAME}_test https)

# add_custom_target(ARGS_TEST COMMAND ${PROJECT_NAME}_test server client)