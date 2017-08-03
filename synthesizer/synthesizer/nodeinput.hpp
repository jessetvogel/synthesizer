#ifndef nodeinput_hpp
#define nodeinput_hpp

#include <string>

class Controller;
class NodeOutput;

class NodeInput {

public:
    
    enum Type { NODE, NODE_VOICE, SAMPLE, CURVE }; // TODO rename
    
private:
    
    Controller* controller;
    
    const Type type;
    std::string expression;
    
public:
    
    NodeInput(Controller*, Type, std::string);
    
    bool autoUpdate;
    
    inline Type getType() { return type; }
    inline std::string getExpression() { return expression; }
    
    bool set(std::string);
    
    NodeOutput* pointer;
    
    static std::string typeToString(Type);

};

#endif
