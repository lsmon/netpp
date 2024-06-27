#include "http/HttpServer.hpp"
#include "http/HttpMethod.hpp"
#include <string.h>

void testHttpServer()
{
    HttpServer serverHttp("localhost", "8080", 1024, 4);
    std::cout << "Server listening on localhost : 8080 " << std::endl;

    // 79612E78-ADD6-47FA-980D-B242A29F0D56 test flag
    serverHttp.setHttpHandler(
        HttpMethod::GET, "/79612E78-ADD6-47FA-980D-B242A29F0D56", [](const HttpRequest &, HttpResponse &response)
        { 
            std::cout << "Hello, World Message response" << std::endl;
            response.setBody("Hello, World!"); 
        });

    serverHttp.run();
}

int main(int argc, char **args)
{
    testHttpServer();
    
    return EXIT_SUCCESS;
}
