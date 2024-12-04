#include "http/Server.hpp"
#include "http/Method.hpp"
#include "api/Client.hpp"
#include "http/Response.hpp"
#include "http/Path.hpp"
#include <string.h>

void testHttpServer()
{
    HttpServer serverHttp("localhost", "8080", 1024, 4);

    serverHttp.setHttpHandler(HttpMethod::GET, "/greetings", [](const HttpRequest &, HttpResponse &response, Path *path)
                              { response.setBody("Hello, World!"); });

    serverHttp.run();
}

void testHttpClient()
{
    try
    {
        ApiClient client;
        std::string url = "http://localhost:8080/greetings";
        HttpResponse response = client.get(url, "");
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
        ApiClient client;
        std::string url = "https://xcapi.alcacruz.com/contest";
        response = client.get(url, "");
        
        std::cout << response.getBody() << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cout << response.getBody() << std::endl;
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}

void testHttpPost() 
{
    HttpResponse response;
    try
    {
        ApiClient client;
        std::string url = "https://es.alcacruz.com/screen/data";
        std::string data = "{\"st\":\"vod\",\"sw\":0,\"wt\":11000,\"fn\":247,\"clid\":\"5B6F52E2-5CBF-4053-829B-405E4E175023\",\"cm\":\"\",\"sid\":\"/10.128.0.2:38830-1\",\"ua\":\"iOS\",\"ra\":\"98.45.246.61\",\"dt\":\"2023-05-23T22:39:59.813Z\",\"dvc\":\"iPad7,3\",\"c\":\"rtqa\",\"api_key\":\"7d3568e5f1126764ac2c249f5240315441d725793d1b7b638f63e3d20769f381012a2cf89ae716f419404be8dd11153be9c5bada35490747faf38b262bd55270\",\"srv\":\"websocket-server-2\",\"vm\":2,\"cid\":\"f64bbe50-ee12-11ed-afad-cb3e8b054e2b\",\"ts\":1684881599813}";
        std::map<std::string, std::string> headers = {{"Authorization", "Basic YWxjYWNydXo6Y29tcHV0ZXJ2aXNpb24="}, {"Content-Type", "application/json"}};

        response = client.post(url, data, headers);
        
        std::cout << response.getBody() << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cout << response.getBody() << std::endl;
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}

void testHttpPostValidateCreds()
{
    HttpResponse response;
    try
    {
        ApiClient client;
        std::string url = "https://api.alcacruz.com/key/api/validate";
        std::string data = "";
        std::map<std::string, std::string> headers = {{"Authorization", "CompetesTV:16552127f5d72f81b488309cb9e28eee8353b8d1a69a556f294009fe2c292d1c6b349fc9550bb1b8589e454447cecf97941736fe0480c332297cd9c9f9f16e48"}, {"Content-Type", "application/json"}};

        response = client.post(url, data, headers);

        std::cout << response.getBody() << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cout << response.getBody() << std::endl;
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}

int main()
{
//    testHttpsClient();
    testHttpPostValidateCreds();
    testHttpPost();
    return EXIT_SUCCESS;
}
