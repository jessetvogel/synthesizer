#ifndef server_hpp
#define server_hpp

#include <iostream>
#include <thread>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "request.hpp"
#include "response.hpp"

class Server {

    int port;
    int queueSize;
    bool (*callback)(Request*, Response*);
    
    int mainSocket;
    struct sockaddr_in serverSocketInfo;
    struct sockaddr_in clientSocketInfo;
    
    bool active;
    void run();
    bool handle(int);
    
public:
    
    Server(int, bool (*)(Request*, Response*), int);
    ~Server();
    
    bool start();
    bool stop();
    
    inline int getPort() { return port; }
    
};

#endif
