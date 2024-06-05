#include "http/HttpServer.hpp"
#include "http/HttpMethod.hpp"
#include "http/HttpClient.hpp"
#include <string.h>

void testHttpServer()
{
    HttpServer serverHttp("localhost", "8080", 1024, 4);

    serverHttp.setHttpHandler(HttpMethod::GET, "/greetings", [](const HttpRequest &, HttpResponse &response)
                              { response.body = "Hello, World!"; });

    serverHttp.run();
}

void testHttpClient()
{
    try
    {
        HttpClient client;
        std::string url = "http://localhost:8080/greetings";
        std::string response = client.Get(url);
        std::cout << "GET Response: " << response << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}


void testHttpsClient()
{
    try
    {
        HttpClient client;
        std::string url = "https://xcapi.alcacruz.com/contest";
        std::string response = client.Get(url);
        std::cout << "GET Response: " << response << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}

int main(int argc, char **args)
{
    if (argc != 2)
    {
        testHttpServer();
        return EXIT_FAILURE;
    }
    if (strcmp(args[1], "server") == 0)
    {
        testHttpServer();
    }
    else if (strcmp(args[1], "client") == 0)
    {
        testHttpClient();
    } 
    else if (strcmp(args[1], "https") == 0)
    {
        testHttpsClient();
    }
    else 
    {
        std::cout << args[1] << std::endl;
    }
    return EXIT_SUCCESS;
}
