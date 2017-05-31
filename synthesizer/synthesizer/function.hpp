#ifndef function_hpp
#define function_hpp

#include <string>

class Controller;

class Function {

public:
    
    enum Type {
        Identity,
        Pow10
    };
    
    static double evaluate(Type, double);
    
    static double identity(double);
    static double pow10(double);
    
    static bool set(Controller*, Type*, std::string);
    
    static double fuzz(double, double);
    
};

#endif
