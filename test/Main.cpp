#include "http/HttpServer.hpp"
#include "http/HttpMethod.hpp"
#include "http/HttpClient.hpp"
#include "http/HttpResponse.hpp"
#include "JSON.hpp"
#include <string.h>
#include "Log.hpp"

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
        LOG_DEBUG << "GET Response: " << response.getBody();
    }
    catch (const std::exception &ex)
    {
        LOG_ERROR << "Error: " << ex.what();
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
        // std::cout << "GET Response: " << response << std::endl;
        auto jsonObj = Util::parse(response.getBody());
        LOG_DEBUG << jsonObj->str();
        // LOG_DEBUG << response.getBody();
    }
    catch (const std::exception &ex)
    {
        LOG_DEBUG << response.getBody();
        LOG_ERROR << "Error: " << ex.what();
    }
}

int main(int argc, char **args)
{
    testHttpsClient();
    return EXIT_SUCCESS;
}
