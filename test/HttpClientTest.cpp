#include "http/HttpMethod.hpp"
#include "http/HttpClient.hpp"
#include <string.h>
#include <iostream>

void testHttpClient()
{
    try
    {
        HttpClient client;
        std::string url = "http://localhost:8080/79612E78-ADD6-47FA-980D-B242A29F0D56";
        HttpResponse response = client.get(url);
        std::cout << "GET Response: " << response.getBody() << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
    }
}

int main(int argc, char **args)
{

    testHttpClient();

    return EXIT_SUCCESS;
}
