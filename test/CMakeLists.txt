find_package(ZLIB REQUIRED)

message(STATUS "CONFIGURING ${PROJECT_NAME}_test")
add_executable(${PROJECT_NAME}_test test/Main.cpp ${HEADERS_SERVER} ${HEADERS_CLIENT})
target_link_libraries(${PROJECT_NAME}_test PRIVATE 
        _${LIB_NAME_SERVER}
        ${LOGPP_LIB}
        ${JSON_LIB}
        _${LIB_NAME_CLIENT}
        ZLIB::ZLIB)
target_include_directories(${PROJECT_NAME}_test PUBLIC 
        ${OPENSSL_INCLUDE_DIR} 
        ${JSON_INC} 
        ${LOGPP_INC}
        _${LIB_NAME_SERVER}
        _${LIB_NAME_CLIENT})
message(STATUS "CONFIGURATION FINISHED: ${PROJECT_NAME}_test")


message(STATUS "CONFIGURING ${PROJECT_NAME}_client_local")
add_executable(${PROJECT_NAME}_client_local
        test/HttpClientTest.cpp
        ${SOURCES_CLIENT}
        ${HEADERS_CLIENT})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
target_link_libraries(${PROJECT_NAME}_client_local PRIVATE
        ${OPENSSL_LIBRARIES}
        ${LOGPP_LIB}
        _${LIB_NAME_CLIENT}
        ZLIB::ZLIB)
else()
target_link_libraries(${PROJECT_NAME}_client_local PRIVATE
        ${OPENSSL_LIBRARIES}
        ${LOGPP_LIB}
        _${LIB_NAME_CLIENT}
        stdc++fs
        ZLIB::ZLIB)
endif()
target_include_directories(${PROJECT_NAME}_client_local PUBLIC
        ${OPENSSL_INCLUDE_DIR}
        ${JSON_INC}
        ${LOGPP_INC}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
message(STATUS "CONFIGURATION FINISHED ${PROJECT_NAME}_client_local")


message(STATUS "CONFIGURING ${PROJECT_NAME}_server_local")
add_executable(${PROJECT_NAME}_server_local test/HttpServerTest.cpp
        ${HEADERS_SERVER}
        ${HEADERS_CLIENT})
target_link_libraries(${PROJECT_NAME}_server_local PRIVATE 
        _${LIB_NAME_SERVER} 
        ${LOGPP_LIB} 
        ZLIB::ZLIB)
target_include_directories(${PROJECT_NAME}_server_local PUBLIC 
        ${OPENSSL_INCLUDE_DIR} 
        ${JSON_INC} 
        ${LOGPP_INC})
message(STATUS "CONFIGURATION FINISHED ${PROJECT_NAME}_server_local")
