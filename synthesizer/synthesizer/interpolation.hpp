#ifndef interpolation_hpp
#define interpolation_hpp

#include <string>

class Controller;

class Interpolation {

public:
    
    enum Type {
        Linear,
        Sine,
        QuartIn,
        QuartOut
    };
    
    static double ease(Type, double);
    
    static double linear(double);
    static double sine(double);
    static double quartIn(double);
    static double quartOut(double);
    
    static bool set(Controller*, Type*, std::string);

};

#endif
