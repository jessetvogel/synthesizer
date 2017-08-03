#ifndef curve_hpp
#define curve_hpp

#include <string>

class Controller;

#define CURVE_LINEAR        (0.0)
#define CURVE_EXPONENTIAL   (1.0)
#define CURVE_SINE          (2.0)
#define CURVE_QUADIN        (3.0)
#define CURVE_QUADOUT       (4.0)
#define CURVE_QUARTIN       (5.0)
#define CURVE_QUARTOUT      (6.0)

class Curve {

public:
    
    static double ease(double, double, double, Curve*);
    
    static void initialize();
    static void destruct();
    
    std::string getId();
    
    static Curve* get(double);
    
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
