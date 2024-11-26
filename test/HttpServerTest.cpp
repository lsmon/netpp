#include "http/Server.hpp"
#include "http/Method.hpp"
#include "http/Path.hpp"
#include <csignal>
#include <cstdlib>
#include <string>

HttpServer *serverHttp;
// Signal handler function
void signalHandler(int signal) {
    switch (signal) {
        case SIGABRT: std::cerr << "Received SIGABRT: Abnormal termination triggered." << std::endl; break;
        case SIGSEGV: std::cerr << "Received SIGSEGV: Segmentation fault occurred." << std::endl; break;
        case SIGINT: std::cerr << "Received SIGINT: Program interrupted (Ctrl+C)." << std::endl; break;
        case SIGILL: std::cerr << "Received SIGILL: Illegal instruction." << std::endl; break;
        case SIGFPE: std::cerr << "Received SIGFPE: Erroneous arithmetic operation." << std::endl; break;
        case SIGTERM: std::cerr << "Received SIGTERM: Termination request." << std::endl; break;
        case SIGHUP: std::cerr << "Received SIGHUP: Hangup." << std::endl; break;
        case SIGQUIT: std::cerr << "Received SIGQUIT: Quit." << std::endl; break;
        case SIGTRAP: std::cerr << "Received SIGTRAP: Trace/breakpoint trap." << std::endl; break;
        case SIGKILL: std::cerr << "Received SIGKILL: Killed." << std::endl; break;
        case SIGBUS: std::cerr << "Received SIGBUS: Bus error." << std::endl; break;
        case SIGSYS: std::cerr << "Received SIGSYS: Bad system call." << std::endl; break;
        case SIGPIPE: std::cerr << "Received SIGPIPE: Broken pipe." << std::endl; break;
        case SIGALRM: std::cerr << "Received SIGALRM: Alarm clock." << std::endl; break;
        case SIGURG: std::cerr << "Urgent data is available at a socket." << std::endl; break;
        case SIGSTOP: std::cerr << "Stop, unblockable." << std::endl; break;
        case SIGTSTP: std::cerr << "Keyboard stop." << std::endl; break;
        case SIGCONT: std::cerr << "Continue." << std::endl; break;
        case SIGCHLD: std::cerr << "Child terminated or stopped." << std::endl; break;
        case SIGTTIN: std::cerr << "Background read from control terminal." << std::endl; break;
        case SIGTTOU: std::cerr << "Background write to control terminal." << std::endl; break;
#ifdef __linux__
        case SIGPOLL: std::cerr << "Pollable event occurred (System V)." << std::endl; break;
#endif
        case SIGXCPU: std::cerr << "CPU time limit exceeded." << std::endl; break;
        case SIGXFSZ: std::cerr << "File size limit exceeded." << std::endl; break;
        case SIGVTALRM: std::cerr << "Virtual timer expired." << std::endl; break;
        case SIGPROF: std::cerr << "Profiling timer expired." << std::endl; break;
        case SIGUSR1: std::cerr << "User-defined signal 1." << std::endl; break;
        case SIGUSR2: std::cerr << "User-defined signal 2." << std::endl; break;
        default: std::cerr << "Received signal: " << signal << std::endl; break;
    }
    // Optionally exit the program after handling the signal
    serverHttp->stop();

    std::exit(signal);
}

void testHttpServer()
{
    serverHttp = new HttpServer("localhost", "8080", 1024, 4);
    std::cout << "Server listening on localhost : 8080 " << std::endl;

    // 79612E78-ADD6-47FA-980D-B242A29F0D56 test flag
    serverHttp->setHttpHandler(
        HttpMethod::GET, "/79612E78-ADD6-47FA-980D-B242A29F0D56", [](const HttpRequest &request, HttpResponse &response, Path* path)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World Message response" << std::endl;
            response.setBody("Hello, World!"); 
        });
    serverHttp->run();
    std::cout << "Server terminated" << std::endl;
}

void testHttpMethodsForServer() {
    serverHttp = new HttpServer("localhost", "8080", 1024, 4);
    std::cout << "Server listening on localhost : 8080 " << std::endl;

    // 79612E78-ADD6-47FA-980D-B242A29F0D56 test flag
    serverHttp->setHttpHandler(
        HttpMethod::GET, "/", [](const HttpRequest &request, HttpResponse &response, Path* path)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World GET Message response" << std::endl;
            response.setBody("Hello, World!");
            response.setStatus(200);
            return ;
        });

    serverHttp->setHttpHandler(
            HttpMethod::GET, "/having/a/nice/{day}", [](const HttpRequest &request, HttpResponse &response, Path* path)
            {
                if (path != nullptr || !path->getPathParamMap().empty())
                    for (auto &item : path->getPathParamMap()) {
                        std::cout << item.first << " : " << item.second << std::endl;
                    }
                std::cout << request.getVersion() << std::endl;
                std::cout << "Hello, World GET Message response" << std::endl;
                response.setBody("Hello, World!");
                response.setStatus(200);
                return ;
            });

    serverHttp->setHttpHandler(
        HttpMethod::POST, "/", [](const HttpRequest &request, HttpResponse &response, Path* path)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World " << HttpMethod::POST << " Message response" << std::endl;
            response.setBody("Hello, World!");
            response.setStatus(200);
            return ;
        });
    
    serverHttp->setHttpHandler(
        HttpMethod::PUT, "/", [](const HttpRequest &request, HttpResponse &response, Path* path)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World " << HttpMethod::PUT << " Message response" << std::endl;
            response.setBody("Hello, World!");
            response.setStatus(200);
            return ;
        });

    serverHttp->setHttpHandler(
        HttpMethod::DELETE, "/", [](const HttpRequest &request, HttpResponse &response, Path* path)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World " << HttpMethod::DELETE << " Message response" << std::endl;
            response.setBody("Hello, World!"); 
            response.setStatus(200);
            return ;
        });

    serverHttp->setHttpHandler(
        HttpMethod::HEAD, "/", [](const HttpRequest &request, HttpResponse &response, Path* path)
        { 
            std::cout << request.getVersion() << std::endl;
            response.addHeader("hello", "world");
            std::cout << "Hello, World " << HttpMethod::HEAD << " Message response" << std::endl;
            response.setStatus(200);
            return ;
        });

    serverHttp->setHttpHandler(
        HttpMethod::OPTIONS, "/", [](const HttpRequest &request, HttpResponse &response, Path* path)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World " << HttpMethod::OPTIONS << " Message response" << std::endl;
            response.setBody("Hello, World!");
            response.setStatus(200);
            return ;
        });

    serverHttp->setHttpHandler(
        HttpMethod::CONNECT, "/", [](const HttpRequest &request, HttpResponse &response, Path* path)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World " << HttpMethod::CONNECT << " Message response" << std::endl;
            response.setBody("Hello, World!"); 
            response.setStatus(200);
            return ;
        });

    serverHttp->setHttpHandler(
        HttpMethod::PATCH, "/", [](const HttpRequest &request, HttpResponse &response, Path* path)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World " << HttpMethod::PATCH << " Message response" << std::endl;
            response.setBody("Hello, World!");
            response.setStatus(200);
            return ;
        });

    serverHttp->setHttpHandler(
        HttpMethod::TRACE, "/", [](const HttpRequest &request, HttpResponse &response, Path* path)
        { 
            std::cout << request.getVersion() << std::endl;
            std::cout << "Hello, World " << HttpMethod::TRACE << " Message response" << std::endl;
            response.setBody("Hello, World!");
            response.setStatus(200);
            return ;
        });
// Register signal handler
    std::signal(SIGABRT, signalHandler);
    std::signal(SIGSEGV, signalHandler);
    std::signal(SIGINT, signalHandler);
    std::signal(SIGKILL, signalHandler);
    serverHttp->run();

    std::cout << "Server terminated" << std::endl;
}

void testPathParsing() {
    // Define patterns
    std::string pathPattern1 = "/path/{key1}/{key2}";
    std::string pathPattern2 = "/path/{key1}/{key2}/path2";
    std::string pathPattern3 = "/path/{key1}/path2/{key2}/path3";
    std::string pathPattern4 = "/path/{key1}";

    Path matcher1(pathPattern1);
    Path matcher2(pathPattern2);
    Path matcher3(pathPattern3);
    Path matcher4(pathPattern4);

    // Test paths
    std::string path1 = "/path/value1/value2";
    std::string path2 = "/path/value1/value2/path2";
    std::string path3 = "/path/value1/path2/value2/path3";
    std::string path4 = "/path/value1";
    std::string path5 = "/path/path2/value1";

    // Store results
    std::optional<std::map<std::string, std::string>> params;

    std::cout << "Matching results:\n";

    // Test path 1
    if (matcher1.match(path1)) {
        params = matcher1.getPathParamMap();
        std::cout << "Path 1 matches pattern 1. Params:\n";
        for (const auto &param : *params) {
            std::cout << param.first << ": " << param.second << "\n";
        }
    } else {
        std::cout << "Path 1 does not match pattern 1.\n";
    }

    // Test path 2
    if (matcher2.match(path2)) {
        params = matcher2.getPathParamMap();
        std::cout << "Path 2 matches pattern 2. Params:\n";
        for (const auto &param : *params) {
            std::cout << param.first << ": " << param.second << "\n";
        }
    } else {
        std::cout << "Path 2 does not match pattern 2.\n";
    }

    // Test path 3
    if (matcher3.match(path3)) {
        params = matcher3.getPathParamMap();
        std::cout << "Path 3 matches pattern 3. Params:\n";
        for (const auto &param : *params) {
            std::cout << param.first << ": " << param.second << "\n";
        }
    } else {
        std::cout << "Path 3 does not match pattern 3.\n";
    }

    // Test path 4
    if (matcher4.match(path4)) {
        params = matcher4.getPathParamMap();
        std::cout << "Path 4 matches pattern 4. Params:\n";
        for (const auto &param : *params) {
            std::cout << param.first << ": " << param.second << "\n";
        }
    } else {
        std::cout << "Path 4 does not match pattern 4.\n";
    }

    // Test path 5 (non-matching case)
    if (matcher4.match(path5)) {
        params = matcher4.getPathParamMap();
        std::cout << "Path 5 matches pattern 4. Params:\n";
        for (const auto &param : *params) {
            std::cout << param.first << ": " << param.second << "\n";
        }
    } else {
        std::cout << "Path 5 does not match pattern 4.\n";
    }
}

int main()
{
    testHttpMethodsForServer();
//    testPathParsing();

    return EXIT_SUCCESS;
}
