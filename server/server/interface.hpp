#ifndef interface_hpp
#define interface_hpp

#include <string>
#include <mutex>

#define NUM_PIPES (2)

class Interface {

    int pipes[NUM_PIPES][2];
    
    std::string readLine();
    
    std::mutex mutex;
    
public:
    
    bool child;
    
    Interface();
    
    bool start();
    bool stop();
    std::string command(std::string);

};

#endif
