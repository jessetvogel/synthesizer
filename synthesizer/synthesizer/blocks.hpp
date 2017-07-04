#ifndef blocks_hpp
#define blocks_hpp

#include <vector>
#include <string>
#include <mutex>

class Controller;
class Block;

class Blocks {

    Controller* controller;
    
    std::vector<Block*> blocks;
    
    std::mutex mutex;
    
public:
    
    Blocks(Controller*);
    ~Blocks();
    
    Block* getBlock(std::string);
    
    // Commands
    bool create(std::string);
    bool destroy(std::string);
    bool addInput(std::string, std::string, std::string, std::string);
    bool addOutput(std::string, std::string, std::string);
    bool attachUnit(std::string, std::string);
    bool set(std::string, std::string, std::string);
  
    // Status
    void printBlocks();
    
};

#endif
