cmake_minimum_required(VERSION 3.11)
project(netpp)
set(CMAKE_CXX_STANDARD 17)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION_TWEAK 0)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}")
set(LIB_NAME "lib${PROJECT_NAME}-${PROJECT_VERSION}")

message(STATUS "INFO -- ${LIB_NAME}")

set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/Server.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/http/HttpMethod.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/http/HttpHandler.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/http/Structures.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/http/HttpRequest.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/http/Status.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/http/QueryString.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/util/UUID.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/util/String.hpp)


set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/Server.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/http/HttpHandler.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/http/QueryString.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/http/HttpRequest.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/util/UUID.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/util/String.cpp)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(${LIB_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${LIB_NAME}
    PUBLIC
        ${OPENSSL_INCLUDE_DIR}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
        
)

target_link_libraries(${LIB_NAME} ${OPENSSL_LIBRARIES})

add_executable(${PROJECT_NAME} src/Main.cpp ${HEADERS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_NAME})