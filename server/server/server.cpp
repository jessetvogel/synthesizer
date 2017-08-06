#include <iostream>

#include "server.hpp"
#include "request.hpp"
#include "response.hpp"
#include "error.hpp"

Server::Server(int port, bool (*callback)(Request*, Response*), int queueSize) {
    this->port = port;
    this->callback = callback;
    this->queueSize = queueSize;
}

Server::~Server() {
    if(active)
        stop();
}

void fail(std::string msg) {
    std::cout << msg << std::endl;
}

bool Server::start() {
    // Open socket
    mainSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(mainSocket < 0) { fail("opening socket"); return false; }
    
    // Initialize socket structures
    memset(&serverSocketInfo, 0, sizeof(serverSocketInfo));
    serverSocketInfo.sin_family = AF_INET;
    serverSocketInfo.sin_addr.s_addr = INADDR_ANY;
    serverSocketInfo.sin_port = htons(port);
    
    // Bind server_socket_information to the opened socket
    int _port = port;
    while(bind(mainSocket, (struct sockaddr*) &serverSocketInfo, sizeof(sockaddr)) < 0) {
        // Try at most 10 different ports
        if(_port - port < 10) {
            _port ++;
            serverSocketInfo.sin_port = htons(_port);
            continue;
        }
        
        fail("binding");
        return false;
    }
    port = _port;

    // Create a new thread to run the server in
    std::thread thread(&Server::run, this);
    thread.detach();
    
    return true;
}

bool Server::stop() {
    active = false;
    if(close(mainSocket) < 0) { fail("closing"); return false; }
    return true;
}

void Server::run() {
    // Start listening
    listen(mainSocket, queueSize);
    
    socklen_t addr_len = sizeof(clientSocketInfo);
    
    active = true;
    while(active) {
        // Accept incoming clients
        int clientSocket = accept(mainSocket, (struct sockaddr*) &clientSocketInfo, &addr_len);
        if(!active) return;
        if(clientSocket < 0) { fail("accepting client"); continue; }
        
        std::thread thread(&Server::handle, this, clientSocket);
        thread.detach();
    }
}

bool Server::handle(int clientSocket) {
    Request request(clientSocket);
    Response response(clientSocket);
    
    if(request.bad())
        return Error::respond(&request, &response, 400);
    
    bool success = callback(&request, &response);
    close(clientSocket);
    return success;
}

