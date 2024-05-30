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

find_package(OpenSSL REQUIRED)

message(STATUS "OPENSSL_INCLUDE_DIR:    ${OPENSSL_INCLUDE_DIR}")
include_directories(${OPENSSL_INCLUDE_DIR})

include(http-server.cmake)

message(STATUS "INFO -- _${LIB_NAME_SERVER}")

include(http-client.cmake)

add_executable(${PROJECT_NAME}_test Main.cpp ${HEADERS_SERVER} ${HEADERS_CLIENT})
target_link_libraries(${PROJECT_NAME}_test PRIVATE _${LIB_NAME_SERVER} ${LOGPP_LIB} _${LIB_NAME_CLIENT})
target_include_directories(${PROJECT_NAME}_test PUBLIC ${OPENSSL_INCLUDE_DIR})

enable_testing()

add_test(TEST ${PROJECT_NAME}_test server client)

# add_custom_target(ARGS_TEST COMMAND ${PROJECT_NAME}_test server client)