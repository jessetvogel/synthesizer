#include "interpolation.hpp"

double Interpolation::ease(Type type, double x) {
    switch(type) {
        case Linear: return linear(x);
        case Sine: return sine(x);
        case QuadIn: return quadIn(x);
        case QuadOut: return quadOut(x);
        case QuartIn: return quartIn(x);
        case QuartOut: return quartOut(x);
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
    
    return false;
}

