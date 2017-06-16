#include "function.hpp"

#include <cmath>

double Function::evaluate(Function* function, double x) {
    if(function == Identity)
        return x;
    
    if(function == Pow10)
        return pow(10.0, x);
    
    return 0.0;
}

double Function::fuzz(double input, double inputGain) {
    return ((input > 0.0) ? 1.0 : ((input < 0.0) ? -1.0 : 0.0)) * (1.0 - std::exp(- inputGain * std::abs(input))) / (1.0 - std::exp(- inputGain));
}

std::string Function::getId() {
    if(this == Identity) return "identity";
    if(this == Pow10) return "pow10";
    return "undefined";
}

Function* Function::Identity;
Function* Function::Pow10;

void Function::initialize() {
    Identity = new Function();
    Pow10 = new Function();
}

void Function::destruct() {
    delete Identity;
    delete Pow10;
}
