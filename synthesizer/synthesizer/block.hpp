#ifndef block_hpp
#define block_hpp

#include <unordered_map>
#include <vector>
#include <string>
#include <mutex>

class Controller;
class Parameter;
class Unit;

class Block {

    Controller* controller;
    
    std::unordered_map<std::string, Parameter*> inputs;
    std::unordered_map<std::string, Unit*> outputs;
    
    std::mutex mutexInputs;
    std::mutex mutexOutputs;
    
    std::string id;
    
public:
    
    Block(Controller*);
    
    inline std::string getId() { return id; }
    inline Block* setId(std::string id) { this->id = id; return this; }
    
    bool addInput(std::string, Parameter*);
    bool addOutput(std::string, Unit*);
    
    Parameter* getInput(std::string);
    Unit* getOutput(std::string);
    
    // Status
    void printBlock();
    
};

#endif
