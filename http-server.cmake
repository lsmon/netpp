
set(LIB_NAME_SERVER "${PROJECT_NAME}-http-server-${PROJECT_VERSION}")

set(INC_SRV
        include/http/ContentType.hpp
        include/http/Server.hpp
        include/http/Method.hpp
        include/http/Handler.hpp
        include/http/Structures.hpp
        include/http/Request.hpp
        include/http/Response.hpp
        include/http/Status.hpp
        include/http/QueryString.hpp
        include/http/WebSocketFrame.hpp
        include/http/Path.hpp)

set(INC_UTIL 
        include/util/URL.hpp
        include/util/UUID.hpp
        include/util/String.hpp)

set(INC include/Exception.hpp
        include/netppconfig.h)

set(HEADERS_SERVER
        ${INC_SRV}
        ${INC_UTIL}
        ${INC})

set(SOURCES_SERVER 
        sources/http/ContentType.cpp
        sources/http/Server.cpp
        sources/http/Handler.cpp
        sources/http/QueryString.cpp
        sources/http/Request.cpp
        sources/http/Response.cpp
        sources/http/Method.cpp
        sources/http/WebSocketFrame.cpp
        sources/util/URL.cpp
        sources/Exception.cpp
        sources/util/UUID.cpp
        sources/util/String.cpp
        sources/http/Path.cpp)

include_directories("include")

add_library(_${LIB_NAME_SERVER} ${HEADERS_SERVER} ${SOURCES_SERVER})

target_include_directories(_${LIB_NAME_SERVER}
    PUBLIC
        ${OPENSSL_INCLUDE_DIR}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(_${LIB_NAME_SERVER} ${OPENSSL_LIBRARIES})
