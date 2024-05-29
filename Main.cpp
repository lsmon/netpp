#include "http/HttpServer.hpp"
#include "http/HttpMethod.hpp"

int main() {
    HttpServer serverHttp("localhost", "8080", 1024, 4);

    serverHttp.setHttpHandler(HttpMethod::GET, "/", [](const HttpRequest&, HttpResponse& response) {
        response.body = "Hello, World!";
    });

    serverHttp.run();
    return 0;
}
