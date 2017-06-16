#ifndef curve_hpp
#define curve_hpp

#include <string>

class Controller;

class Curve {

public:
    
    static double ease(double, double, double, Curve*);
    
    static void initialize();
    static void destruct();
    
    std::string getId();
    
    static Curve* Linear;
    static Curve* Sine;
    static Curve* QuadIn;
    static Curve* QuadOut;
    static Curve* QuartIn;
    static Curve* QuartOut;
    static Curve* Exponential;
    
    static double linear(double);
    static double sine(double);
    static double quadIn(double);
    static double quadOut(double);
    static double quartIn(double);
    static double quartOut(double);

};

#endif
