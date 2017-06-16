#ifndef function_hpp
#define function_hpp

#include <string>

class Controller;

class Function {

public:
    
    static double evaluate(Function*, double);
    
    static void initialize();
    static void destruct();
    
    std::string getId();
    
    static Function* Identity;
    static Function* Pow10;
    
    static double fuzz(double, double);
    
};

#endif
