
set(LIB_NAME_SERVER "${PROJECT_NAME}-http-server-${PROJECT_VERSION}")

set(INC_SRV 
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/ContentType.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/Server.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/Method.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/Handler.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/Structures.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/Request.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/Response.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/Status.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/QueryString.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/http/WebSocketFrame.hpp
        include/util/URL.hpp)

set(INC_UTIL 
            ${CMAKE_CURRENT_SOURCE_DIR}/include/Exception.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/util/UUID.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/include/util/String.hpp)

set(HEADERS_SERVER ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h
            ${INC_SRV}
            ${INC_UTIL})

set(SOURCES_SERVER 
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/http/ContentType.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/http/Server.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/http/Handler.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/http/QueryString.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/http/Request.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/http/Response.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/http/Method.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/http/WebSocketFrame.cpp
        sources/util/URL.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/Exception.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/util/UUID.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/sources/util/String.cpp)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(_${LIB_NAME_SERVER} ${HEADERS_SERVER} ${SOURCES_SERVER})

target_include_directories(_${LIB_NAME_SERVER}
    PUBLIC
        ${OPENSSL_INCLUDE_DIR}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(_${LIB_NAME_SERVER} ${OPENSSL_LIBRARIES})
