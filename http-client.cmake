
set(LIB_NAME_CLIENT "${PROJECT_NAME}-http-client-${PROJECT_VERSION}")

set(HEADERS_CLIENT include/http/HttpClient.hpp)
set(SOURCES_CLIENT sources/http/HttpClient.cpp)

add_library(_${LIB_NAME_CLIENT} ${SOURCES_CLIENT})
