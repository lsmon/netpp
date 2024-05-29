
set(LIB_NAME_SERVER "${PROJECT_NAME}-http-server-${PROJECT_VERSION}")


message(STATUS "INFO -- ${LIB_NAME}")
message(STATUS "INFO -- CONFIGURE START")
include(${CMAKE_CURRENT_SOURCE_DIR}/external/CMakeLists.txt)
message(STATUS "INFO -- CONFIGURE FINISHED")

set(HEADERS_SERVER ${CMAKE_CURRENT_SOURCE_DIR}/include/http/HttpServer.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/HttpMethod.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/HttpHandler.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/Structures.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/HttpRequest.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/Status.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/QueryString.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/util/UUID.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/util/String.hpp)


set(SOURCES_SERVER ${CMAKE_CURRENT_SOURCE_DIR}/sources/http/HttpServer.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/http/HttpHandler.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/http/QueryString.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/http/HttpRequest.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/util/UUID.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/util/String.cpp)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(_${LIB_NAME_SERVER} ${HEADERS_SERVER} ${SOURCES_SERVER} ${LOGPP_INC})

target_include_directories(_${LIB_NAME_SERVER}
    PUBLIC
        ${OPENSSL_INCLUDE_DIR}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${LOGPP_INC}
)

target_link_libraries(_${LIB_NAME_SERVER} ${OPENSSL_LIBRARIES})

add_executable(${PROJECT_NAME}-http-server Main.cpp ${HEADERS})
target_link_libraries(${PROJECT_NAME}-http-server PRIVATE _${LIB_NAME_SERVER} ${LOGPP_LIB})