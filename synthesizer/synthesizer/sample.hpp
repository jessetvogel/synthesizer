#ifndef sample_hpp
#define sample_hpp

#include <string>

class Controller;

class Sample {

    int resolution;
    int periods;
    double* data;
    
public:
    
    Sample(int, int, double*);
    ~Sample();
    
    float getValue(double);
    
    static void initialize();
    static void destruct();

    static Sample* sine;
    static Sample* square;
    static Sample* triangle;
    static Sample* sawtooth;
    
    static bool set(Controller*, Sample**, std::string);
};

#endif
