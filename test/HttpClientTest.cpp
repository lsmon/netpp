#include "http/HttpMethod.hpp"
#include "http/HttpClient.hpp"
#include <string.h>
#include <iostream>
#include "Log.hpp"

void testHttpClient()
{
    try
    {
        HttpClient client;
        std::string url = "http://localhost:8080/79612E78-ADD6-47FA-980D-B242A29F0D56";
        HttpResponse response = client.get(url);
        LOG_DEBUG << "GET Response: " << response.getBody();
    }
    catch (const std::exception &ex)
    {
        LOG_ERROR << "Error: " << ex.what();
    }
}

int main(int argc, char **args)
{

    testHttpClient();

    return EXIT_SUCCESS;
}
