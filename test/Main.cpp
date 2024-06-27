#include "http/HttpServer.hpp"
#include "http/HttpMethod.hpp"
#include "http/HttpClient.hpp"
#include "http/HttpResponse.hpp"
#include <string.h>

void testHttpServer()
{
    HttpServer serverHttp("localhost", "8080", 1024, 4);

    serverHttp.setHttpHandler(HttpMethod::GET, "/greetings", [](const HttpRequest &, HttpResponse &response)
                              { response.setBody("Hello, World!"); });

    serverHttp.run();
}

void testHttpClient()
{
    try
    {
        HttpClient client;
        std::string url = "http://localhost:8080/greetings";
        HttpResponse response = client.get(url);
        std::cout << "GET Response: " << response.getBody() << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what();
    }
}


void testHttpsClient()
{
    HttpResponse response;
    try
    {
        HttpClient client;
        std::string url = "https://xcapi.alcacruz.com/contest";
        response = client.get(url);
        
        std::cout << response.getBody() << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cout << response.getBody() << std::endl;
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}

int main(int argc, char **args)
{
    testHttpsClient();
    return EXIT_SUCCESS;
}
