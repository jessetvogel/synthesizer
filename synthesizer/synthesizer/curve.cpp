#include <cmath>

#include "curve.hpp"

double Curve::ease(double min, double max, double x, Curve* curve) {
    double f;
    if(x < 0) x = 0.0;
    if(x > 1) x = 1.0;
    
    if(curve == Linear) {
        f = linear(x);
        return min * (1.0 - f) + max * f;
    }
    
    if(curve == Sine) {
        f = sine(x);
        return min * (1.0 - f) + max * f;
    }
    
    if(curve == QuadIn) {
        f = quadIn(x);
        return min * (1.0 - f) + max * f;
    }
    
    if(curve == QuadOut) {
        f = quadOut(x);
        return min * (1.0 - f) + max * f;
    }
    
    if(curve == QuartIn) {
        f = quartIn(x);
        return min * (1.0 - f) + max * f;
    }
    
    if(curve == QuartOut) {
        f = quartOut(x);
        return min * (1.0 - f) + max * f;
    }
    
    if(curve == Exponential) {
        if(min <= 0 || max <= 0) return 1.0;
        double a = log(min);
        double b = log(max);
        return exp(a * (1.0 - x) + b * x);
    }
    
    return 0.0;
}

double Curve::linear(double x) {
    return x;
}

double Curve::sine(double x) {
    return 3.0*x*x - 2.0*x*x*x;
}

double Curve::quadIn(double x) {
    return x*x;
}

double Curve::quadOut(double x) {
    x -= 1.0;
    return 1.0 - x*x;
}

double Curve::quartIn(double x) {
    return x*x*x*x;
}

double Curve::quartOut(double x) {
    x -= 1.0;
    return 1.0 - x*x*x*x;
}

std::string Curve::getId() {
    if(this == Linear) return "linear";
    if(this == Sine) return "sine";
    if(this == QuadIn) return "quadin";
    if(this == QuadOut) return "quadout";
    if(this == QuartIn) return "quartin";
    if(this == QuartOut) return "quartout";
    if(this == Exponential) return "exponential";
    return "undefined";
}

Curve* Curve::get(double x) {
    if(x >= CURVE_LINEAR && x < CURVE_LINEAR + 1.0)             return Linear;
    if(x >= CURVE_EXPONENTIAL && x < CURVE_EXPONENTIAL + 1.0)   return Exponential;
    if(x >= CURVE_SINE && x < CURVE_SINE + 1.0)                 return Sine;
    if(x >= CURVE_QUADIN && x < CURVE_QUADIN + 1.0)             return QuadIn;
    if(x >= CURVE_QUADOUT && x < CURVE_QUADOUT + 1.0)           return QuadOut;
    if(x >= CURVE_QUARTIN && x < CURVE_QUARTIN + 1.0)           return QuartIn;
    if(x >= CURVE_QUARTOUT && x < CURVE_QUARTOUT + 1.0)         return QuartOut;
    
    return Linear; // TODO
}

Curve* Curve::Linear;
Curve* Curve::Sine;
Curve* Curve::QuadIn;
Curve* Curve::QuadOut;
Curve* Curve::QuartIn;
Curve* Curve::QuartOut;
Curve* Curve::Exponential;

void Curve::initialize() {
    Linear = new Curve();
    Sine = new Curve();
    QuadIn = new Curve();
    QuadOut = new Curve();
    QuartIn = new Curve();
    QuartOut = new Curve();
    Exponential = new Curve();
}

void Curve::destruct() {
    delete Linear;
    delete Sine;
    delete QuadIn;
    delete QuadOut;
    delete QuartIn;
    delete QuartOut;
    delete Exponential;
}
