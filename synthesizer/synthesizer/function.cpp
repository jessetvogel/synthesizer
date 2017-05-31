#include "function.hpp"

#include <cmath>

double Function::evaluate(Type type, double x) {
    switch(type) {
        case Identity: return identity(x);
        case Pow10: return pow10(x);
    }
}

double Function::identity(double x) {
    return x;
}

double Function::pow10(double x) {
    return pow(10.0, x);
}

bool Function::set(Controller* controller, Type* parameterAddr, std::string value) {
    if(value.compare("identity") == 0) { *parameterAddr = Identity; return true; }
    if(value.compare("pow10") == 0) { *parameterAddr = Pow10; return true; }
    
    return false;
}

double Function::fuzz(double input, double inputGain) {
    return ((input > 0.0) ? 1.0 : ((input < 0.0) ? -1.0 : 0.0)) * (1.0 - std::exp(- inputGain * std::abs(input))) / (1.0 - std::exp(- inputGain));
}
