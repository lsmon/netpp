
set(LIB_NAME_CLIENT "${PROJECT_NAME}-http-client-${PROJECT_VERSION}")

set(INC_CLT include/http/Client.hpp include/config.h)
set(HEADERS_CLIENT ${INC_CLT})
set(SOURCES_CLIENT sources/http/Client.cpp)

add_library(_${LIB_NAME_CLIENT} ${SOURCES_CLIENT} ${HEADERS_CLIENT})

target_include_directories(_${LIB_NAME_CLIENT}
    PUBLIC
        ${OPENSSL_INCLUDE_DIR}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(_${LIB_NAME_CLIENT} ${OPENSSL_LIBRARIES})
