#ifndef sample_hpp
#define sample_hpp

#include <string>

class Sample {

    int resolution;
    int periods;
    double* data;
    
public:
    
    Sample(int, int, double*);
    ~Sample();
    
    float getValue(double);
    
    static void initialize();
    static Sample* sine;
    static Sample* square;
    static Sample* triangle;
    static Sample* sawtooth;
    
    static Sample* fromString(std::string);
};

#endif
