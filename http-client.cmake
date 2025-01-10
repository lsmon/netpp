
set(LIB_NAME_CLIENT "${PROJECT_NAME}-http-client-${PROJECT_VERSION}")

set(INC_HTTP_CLT
        include/http/Method.hpp
        include/http/Response.hpp)

set(INC_API_CLT include/api/Client.hpp)

set(INC_CLT_UTIL
        include/util/URL.hpp
        include/util/String.hpp)

set(INC include/Exception.hpp
        include/netppconfig.h)

set(HEADERS_CLIENT ${INC} ${INC_CLT_UTIL} ${INC_API_CLT} ${INC_HTTP_CLT})

set(SOURCES_CLIENT
        sources/api/Client.cpp
        sources/http/Method.cpp
        sources/http/Response.cpp
        sources/util/URL.cpp
        sources/util/String.cpp
        sources/Exception.cpp)

add_library(_${LIB_NAME_CLIENT} ${SOURCES_CLIENT} ${HEADERS_CLIENT})

target_include_directories(_${LIB_NAME_CLIENT}
    PUBLIC
        ${OPENSSL_INCLUDE_DIR}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(_${LIB_NAME_CLIENT} ${OPENSSL_LIBRARIES})
