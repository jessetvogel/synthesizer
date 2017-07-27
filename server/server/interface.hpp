#ifndef interface_hpp
#define interface_hpp

#include <string>
#include <mutex>

#define NUM_PIPES (2)

class Interface {

    int pipes[NUM_PIPES][2];
    
    std::string readLine();
    
    std::mutex mutexIO;
    std::mutex mutexRunning;
    
public:
    
    Interface();
    
    bool start();
    bool restart();
    bool stop();
    std::string command(std::string);
    
    void wait();
};

#endif
