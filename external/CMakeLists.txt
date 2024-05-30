# Adding logpp project
message(STATUS "Project root path: ${CMAKE_SOURCE_DIR}")

message(STATUS "logpp installing")
set(LOGPP_V "0.9.2.5")

# set(logpp ${CMAKE_SOURCE_DIR}/logpp)
# if (NOT EXISTS ${logpp})
    message(STATUS "logpp is NOT installed")
    execute_process(COMMAND python3 ${CMAKE_SOURCE_DIR}/external/install-scripts/install_logpp.py ${CMAKE_SOURCE_DIR} ${LOGPP_V})
# else ()
    # message(STATUS "logpp is installed")
# endif ()

set(LOGPP_INC ${CMAKE_SOURCE_DIR}/include)
set(LOGPP_LIB ${CMAKE_SOURCE_DIR}/lib/lib_logpp-${LOGPP_V}.a)


# Adding json project
message(STATUS "Project root path: ${CMAKE_SOURCE_DIR}")

message(STATUS "json installing")
set(JSON_V "0.3.0.0")
# set(json ${CMAKE_SOURCE_DIR}/json)
# if (NOT EXISTS ${json})
    message(STATUS "json is NOT installed")
    execute_process(COMMAND python3 ${CMAKE_SOURCE_DIR}/external/install-scripts/install_json.py ${CMAKE_SOURCE_DIR} ${JSON_V})
# else ()
    # message(STATUS "logpp is installed")
# endif ()

set(JSON_INC ${CMAKE_SOURCE_DIR}/include)
set(JSON_LIB ${CMAKE_SOURCE_DIR}/lib/lib_json-${JSON_V}.a)

