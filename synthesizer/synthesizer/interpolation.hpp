#ifndef interpolation_hpp
#define interpolation_hpp

#include <string>

class Controller;

class Interpolation {

public:
    
    enum Type {
        Linear,
        Sine,
        QuadIn,
        QuadOut,
        QuartIn,
        QuartOut,
        Exponential
    };
    
    static double ease(double, double, double, Type);
    
    static double linear(double);
    static double sine(double);
    static double quadIn(double);
    static double quadOut(double);
    static double quartIn(double);
    static double quartOut(double);
    
    static bool set(Controller*, Type*, std::string);

};

#endif
