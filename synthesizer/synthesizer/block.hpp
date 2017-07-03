#ifndef block_hpp
#define block_hpp

#include <unordered_map>
#include <vector>
#include <string>

class Controller;
class Parameter;
class Unit;

class Block {

    Controller* controller;
    
    std::unordered_map<std::string, Parameter*> inputs;
    std::unordered_map<std::string, Unit*> outputs;
    std::vector<Unit*> attachments;
    
public:
    
    Block(Controller*);
    ~Block();
    
    bool addInput(std::string, Parameter*);
    bool addOutput(std::string, Unit*);
    bool attachUnit(Unit*);
    
};

#endif
