#ifndef parameter_hpp
#define parameter_hpp

#include <string>

class Controller;

class Parameter {

public:
    
    enum Type { UNIT, UNIT_KEY_INDEPENDENT, SAMPLE, CURVE, FUNCTION };
    
private:
    
    Controller* controller;
    
public:
    
    void* pointer;
    const Type type;
    const std::string label;
    
    Parameter(Controller*, Type, std::string, std::string);
    
    bool set(std::string);
    
    std::string valueToString();
    
    static std::string typeToString(Type);

};

#endif
