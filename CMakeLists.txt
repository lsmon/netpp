cmake_minimum_required(VERSION 3.11)
project(netpp)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION_TWEAK 3)

set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}")

message(STATUS "INFO -- CONFIGURE START")
message("Operating System: ${CMAKE_SYSTEM_NAME}")
message("Operating System Version: ${CMAKE_SYSTEM_VERSION}")
message("Processor Architecture: ${CMAKE_SYSTEM_PROCESSOR}")

find_package(OpenSSL REQUIRED)

if (OpenSSL_FOUND)
    set(OPENSSL_ENABLED true)
    message(STATUS "OPENSSL_INCLUDE_DIR:    ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "OPENSSL_LIBRARIES:      ${OPENSSL_LIBRARIES}")

    include_directories(${OPENSSL_INCLUDE_DIR})
    
else()
    set(OPENSSL_ENABLED false)
endif()

find_package(Threads REQUIRED)

configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/netppconfig.h)
include(external/CMakeLists.txt)
include(http-server.cmake)
include(http-client.cmake)

message(STATUS "INFO -- _${LIB_NAME_SERVER}")
message(STATUS "INFO -- _${LIB_NAME_CLIENT}")
message(STATUS "INFO -- CONFIGURE FINISHED")

# Optionally allow the user to disable testing
option(BUILD_TESTING "Build the testing tree." ON)

enable_testing()

message(STATUS "INFO -- BUILD_TESTING: ${BUILD_TESTING}")

if (BUILD_TESTING)
    include(test/CMakeLists.txt)
endif()

set(LIB_PROJECT "${PROJECT_NAME}-${PROJECT_VERSION}")

add_library(_${LIB_PROJECT} ${HEADERS_SERVER} ${SOURCES_SERVER} ${SOURCES_CLIENT} ${HEADERS_CLIENT} ${SOURCES_LRU_CACHE} ${INC_LRU_CACHE})

target_include_directories(_${LIB_PROJECT}
    PUBLIC
        ${OPENSSL_INCLUDE_DIR}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(_${LIB_PROJECT} ${OPENSSL_LIBRARIES} stdc++fs)

# add_test(TEST_SERVER ${PROJECT_NAME}_test server)
# add_test(TEST_CLIENT ${PROJECT_NAME}_test https)

include(cpack/CMakeLists.txt)