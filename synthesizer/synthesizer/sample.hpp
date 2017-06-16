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
    
    static void initialize();
    static void destruct();

    float getValue(double);
    std::string getId();
    
    static Sample* sine;
    static Sample* square;
    static Sample* triangle;
    static Sample* sawtooth;
};

#endif
