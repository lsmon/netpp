find_package(ZLIB REQUIRED)

add_executable(${PROJECT_NAME}_test test/Main.cpp ${HEADERS_SERVER} ${HEADERS_CLIENT})
target_link_libraries(${PROJECT_NAME}_test PRIVATE _${LIB_NAME_SERVER} ${LOGPP_LIB} ${JSON_LIB} _${LIB_NAME_CLIENT} ZLIB::ZLIB)
target_include_directories(${PROJECT_NAME}_test PUBLIC ${OPENSSL_INCLUDE_DIR} ${JSON_INC} ${LOGPP_INC})

add_executable(${PROJECT_NAME}_client_local test/HttpClientTest.cpp ${HEADERS_SERVER} ${HEADERS_CLIENT})
target_link_libraries(${PROJECT_NAME}_client_local PRIVATE _${LIB_NAME_SERVER} ${LOGPP_LIB} _${LIB_NAME_CLIENT} ZLIB::ZLIB)
target_include_directories(${PROJECT_NAME}_client_local PUBLIC ${OPENSSL_INCLUDE_DIR} ${JSON_INC} ${LOGPP_INC})


add_executable(${PROJECT_NAME}_server_local test/HttpServerTest.cpp ${HEADERS_SERVER} ${HEADERS_CLIENT})
target_link_libraries(${PROJECT_NAME}_server_local PRIVATE _${LIB_NAME_SERVER} ${LOGPP_LIB} _${LIB_NAME_CLIENT} ZLIB::ZLIB)
target_include_directories(${PROJECT_NAME}_server_local PUBLIC ${OPENSSL_INCLUDE_DIR} ${JSON_INC} ${LOGPP_INC})
