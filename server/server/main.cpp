#include <iostream>
#include <string>

#include "server.hpp"
#include "interface.hpp"
#include "api.hpp"
#include "web.hpp"
#include "instrument.hpp"
#include "error.hpp"

bool callback(Request* request, Response* response) {
    if(API::handle(request, response)) return true;
    if(Web::handle(request, response)) return true;
    if(Instrument::handle(request, response)) return true;
    
    // Show some request information
    std::cout << "----------------" << std::endl;
    std::cout << "Method: " << request->getMethod() << std::endl;
    std::cout << "Request URI: " << request->getRequestURI() << std::endl;
    std::cout << "HTTP version: " << request->getHTTPVersion() << std::endl;
    std::cout << "Host: " << request->getHeader("Host") << std::endl << std::endl;
    std::cout << "----------------" << std::endl;
    
    return Error::respond(request, response, 400);
}

int main(int argc, const char * argv[]) {
    // Create interface
    Interface interface;
    
    // Set references to interface
    API::interface = &interface;
    Instrument::interface = &interface;
    
    // Start interface
    interface.start();
    
    // Create and start server
    Server server(8080, &callback, 10);
    if(!server.start()) return 0;

        std::cout << "Started on port " << server.getPort() << std::endl;
    
    // Wait for input
    std::string line;
    while(std::getline(std::cin, line)) {
        // Check for quit command
        if(line.compare("quit") == 0) {
            std::cout << "received quit" << std::endl;
            break;
        }
    }
    
    std::cout << "quitting in " << (interface.child ? "child" : "parent") << " process" << std::endl;
    
    // Stop server
    server.stop();
    interface.stop();
    
    return 0;
}
