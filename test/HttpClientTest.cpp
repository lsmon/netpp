
#include "api/Client.hpp"
#include <iostream>

void testHttpClient()
{
    try
    {
        ApiClient client;
        std::string url = "http://localhost:8080/79612E78-ADD6-47FA-980D-B242A29F0D56";
        auto response = client.get(url, "");
        std::cout << "GET Status: " << std::to_string(response.getStatus()) << "\n";
        std::cout << "GET Body: " << response.getBody() << "\n";
//        std::cout << "GET Response: " << response << "\n";
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}

void testHttpGetApiClient()
{
    try
    {
        ApiClient client;
        std::string url = "https://postman-echo.com/get?foo1=bar1&foo2=bar2";
        auto response = client.get(url, "");
        std::cout << "GET Status: " << std::to_string(response.getStatus()) << "\n";
        std::cout << "GET Body: " << response.getBody() << "\n";
//        std::cout << "GET Response: " << response << "\n";
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}

void testHttpPostApiClient()
{
    try
    {
        ApiClient client;
        std::string postData = "{\"title\": \"foo\", \"body\": \"bar\", \"userId\": 1}";
        std::map<std::string, std::string> headers = {
                {"Content-Type", "application/json"}
        };
        std::string url = "https://postman-echo.com/post";
        auto response = client.post(url, postData, headers);

        std::cout << "GET Status: " << std::to_string(response.getStatus()) << "\n";
        std::cout << "GET Body: " << response.getBody() << "\n";
//        std::cout << "GET Response: " << response << "\n";
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}

int main()
{

    //testHttpClient();

    testHttpGetApiClient();
    testHttpPostApiClient();
    return EXIT_SUCCESS;
}
