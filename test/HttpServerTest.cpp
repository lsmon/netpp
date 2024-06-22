#include "http/HttpServer.hpp"
#include "http/HttpMethod.hpp"
#include <string.h>
#include "Log.hpp"

void testHttpServer()
{
    HttpServer serverHttp("localhost", "8080", 1024, 4);
    LOG_INFO << "Server listening on localhost : 8080 ";

    // 79612E78-ADD6-47FA-980D-B242A29F0D56 test flag
    serverHttp.setHttpHandler(
        HttpMethod::GET, "/79612E78-ADD6-47FA-980D-B242A29F0D56", [](const HttpRequest &, HttpResponse &response)
        { 
            LOG_DEBUG << "Hello, World Message response";
            response.setBody("Hello, World!"); 
        });

    serverHttp.run();
}

int main(int argc, char **args)
{
    testHttpServer();
    
    return EXIT_SUCCESS;
}
