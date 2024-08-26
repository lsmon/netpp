#include "http/HttpServer.hpp"
#include "http/HttpMethod.hpp"
#include <string.h>

void testHttpServer()
{
    HttpServer serverHttp("localhost", "8080", 1024, 4);
    std::cout << "Server listening on localhost : 8080 " << std::endl;

    // 79612E78-ADD6-47FA-980D-B242A29F0D56 test flag
    serverHttp.setHttpHandler(
        HttpMethod::GET, "/79612E78-ADD6-47FA-980D-B242A29F0D56", [](const HttpRequest &request, HttpResponse &response)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World Message response" << std::endl;
            response.setBody("Hello, World!"); 
        });
    serverHttp.run();
    std::cout << "Server terminated" << std::endl;
}

void testHttpMethodsForServer() {
    HttpServer serverHttp("localhost", "8080", 1024, 4);
    std::cout << "Server listening on localhost : 8080 " << std::endl;

    // 79612E78-ADD6-47FA-980D-B242A29F0D56 test flag
    serverHttp.setHttpHandler(
        HttpMethod::GET, "/", [](const HttpRequest &request, HttpResponse &response)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World GET Message response" << std::endl;
            response.setBody("Hello, World!"); 
        });

    serverHttp.setHttpHandler(
        HttpMethod::POST, "/", [](const HttpRequest &request, HttpResponse &response)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World " << HttpMethod::GET << " Message response" << std::endl;
            response.setBody("Hello, World!"); 
        });
    
    serverHttp.setHttpHandler(
        HttpMethod::PUT, "/", [](const HttpRequest &request, HttpResponse &response)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World " << HttpMethod::PUT << " Message response" << std::endl;
            response.setBody("Hello, World!"); 
        });

    serverHttp.setHttpHandler(
        HttpMethod::DELETE, "/", [](const HttpRequest &request, HttpResponse &response)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World " << HttpMethod::DELETE << " Message response" << std::endl;
            response.setBody("Hello, World!"); 
        });

    serverHttp.setHttpHandler(
        HttpMethod::HEAD, "/", [](const HttpRequest &request, HttpResponse &response)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World " << HttpMethod::HEAD << " Message response" << std::endl;
        });

    serverHttp.setHttpHandler(
        HttpMethod::OPTIONS, "/", [](const HttpRequest &request, HttpResponse &response)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World " << HttpMethod::OPTIONS << " Message response" << std::endl;
            response.setBody("Hello, World!"); 
        });

    serverHttp.setHttpHandler(
        HttpMethod::CONNECT, "/", [](const HttpRequest &request, HttpResponse &response)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World " << HttpMethod::CONNECT << " Message response" << std::endl;
            response.setBody("Hello, World!"); 
        });

    serverHttp.setHttpHandler(
        HttpMethod::PATCH, "/", [](const HttpRequest &request, HttpResponse &response)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World " << HttpMethod::PATCH << " Message response" << std::endl;
            response.setBody("Hello, World!"); 
        });

    serverHttp.setHttpHandler(
        HttpMethod::TRACE, "/", [](const HttpRequest &request, HttpResponse &response)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World " << HttpMethod::TRACE << " Message response" << std::endl;
            response.setBody("Hello, World!"); 
        });
    
    serverHttp.run();

    std::cout << "Server terminated" << std::endl;
}

int main()
{
    testHttpServer();
    
    return EXIT_SUCCESS;
}
