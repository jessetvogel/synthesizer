#ifndef nodeinput_hpp
#define nodeinput_hpp

#include <string>

class Controller;

class NodeInput {

public:
    
    enum Type { NODE, NODE_KEY_INDEPENDENT, SAMPLE, CURVE, FUNCTION };
    
private:
    
    Controller* controller;
    
    const Type type;
    std::string expression;
    
public:
    
    NodeInput(Controller*, Type, std::string);
    
    inline Type getType() { return type; }
    inline std::string getExpression() { return expression; }
    
    bool set(std::string);
    
    void* pointer;
    
    static std::string typeToString(Type);

};

#endif
