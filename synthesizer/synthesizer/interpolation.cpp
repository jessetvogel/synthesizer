#include "interpolation.hpp"

double Interpolation::ease(Type type, double x) {
    switch(type) {
        case Linear: return linear(x);
        case Sine: return sine(x);
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

double Interpolation::quartIn(double x) {
    return x*x*x*x;
}

double Interpolation::quartOut(double x) {
    x -= 1.0;
    return 1.0 - x*x*x*x;
}

bool Interpolation::set(Controller* controller, Type* type, std::string value) {
    if(value.compare("linear") == 0) { *type = Linear; return true; }
    if(value.compare("sine") == 0) { *type = Sine; return true; }
    if(value.compare("quartin") == 0) { *type = QuartIn; return true; }
    if(value.compare("quartout") == 0) { *type = QuartOut; return true; }
    
    return false;
}

