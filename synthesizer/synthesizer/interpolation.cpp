#include <cmath>

#include "interpolation.hpp"

double Interpolation::ease(double min, double max, double x, Type type) {
    double f;
    switch(type) {
        case Linear:
            f = linear(x);
            return min * (1.0 - f) + max * f;
            
        case Sine:
            f = sine(x);
            return min * (1.0 - f) + max * f;
            
        case QuadIn:
            f = quadIn(x);
            return min * (1.0 - f) + max * f;
            
        case QuadOut:
            f = quadOut(x);
            return min * (1.0 - f) + max * f;
            
        case QuartIn:
            f = quartIn(x);
            return min * (1.0 - f) + max * f;
            
        case QuartOut:
            f = quartOut(x);
            return min * (1.0 - f) + max * f;
            
        case Exponential:
            if(min <= 0 || max <= 0) return 1.0;
            double a = log(min);
            double b = log(max);
            return exp(a * (1.0 - x) + b * x);
    }
}

double Interpolation::linear(double x) {
    return x;
}

double Interpolation::sine(double x) {
    return 3.0*x*x - 2.0*x*x*x;
}

double Interpolation::quadIn(double x) {
    return x*x;
}

double Interpolation::quadOut(double x) {
    x -= 1.0;
    return 1.0 - x*x;
}

double Interpolation::quartIn(double x) {
    return x*x*x*x;
}

double Interpolation::quartOut(double x) {
    x -= 1.0;
    return 1.0 - x*x*x*x;
}

bool Interpolation::set(Controller* controller, Type* parameterAddr, std::string value) {
    if(value.compare("linear") == 0) { *parameterAddr = Linear; return true; }
    if(value.compare("sine") == 0) { *parameterAddr = Sine; return true; }
    if(value.compare("quadin") == 0) { *parameterAddr = QuadIn; return true; }
    if(value.compare("quadout") == 0) { *parameterAddr = QuadOut; return true; }
    if(value.compare("quartin") == 0) { *parameterAddr = QuartIn; return true; }
    if(value.compare("quartout") == 0) { *parameterAddr = QuartOut; return true; }
    if(value.compare("exponential") == 0) { *parameterAddr = Exponential; return true; }
    
    return false;
}

