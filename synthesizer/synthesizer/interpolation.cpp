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

Interpolation::Type Interpolation::fromString(std::string str) {
    if(str.compare("linear") == 0) return Linear;
    if(str.compare("sine") == 0) return Sine;
    if(str.compare("quartin") == 0) return QuartIn;
    if(str.compare("quartout") == 0) return QuartOut;
    
    // TODO: what to do if no match?
    return Linear;
}

