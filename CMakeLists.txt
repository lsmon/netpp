cmake_minimum_required(VERSION 3.11)
project(netpp)
set(CMAKE_CXX_STANDARD 17)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION_TWEAK 0)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}")
set(LIB_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
#  set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX14.5.sdk")
#  message(STATUS "CMAKE_OSX_SYSROOT: ${CMAKE_OSX_SYSROOT}")

message(STATUS "INFO -- ${LIB_NAME}")
message(STATUS "INFO -- CONFIGURE START")
include(${CMAKE_CURRENT_SOURCE_DIR}/external/CMakeLists.txt)
message(STATUS "INFO -- CONFIGURE FINISHED")

set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/http/HttpServer.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/HttpMethod.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/HttpHandler.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/Structures.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/HttpRequest.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/Status.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/QueryString.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/util/UUID.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/util/String.hpp)


set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sources/http/HttpServer.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/http/HttpHandler.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/http/QueryString.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/http/HttpRequest.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/util/UUID.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/util/String.cpp)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(_${LIB_NAME} ${HEADERS} ${SOURCES} ${LOGPP_INC})

target_include_directories(_${LIB_NAME}
    PUBLIC
        ${OPENSSL_INCLUDE_DIR}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${LOGPP_INC}
        
)

target_link_libraries(_${LIB_NAME} ${OPENSSL_LIBRARIES})

add_executable(${PROJECT_NAME} Main.cpp ${HEADERS})
target_link_libraries(${PROJECT_NAME} PRIVATE _${LIB_NAME} ${LOGPP_LIB})