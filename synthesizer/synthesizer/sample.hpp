#ifndef sample_hpp
#define sample_hpp

#include <string>

#define SAMPLE_SINE         (0.0)
#define SAMPLE_TRIANGLE     (1.0)
#define SAMPLE_SAWTOOTH     (2.0)
#define SAMPLE_SQUARE       (3.0)

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
    
    static Sample* get(double);
    
    static Sample* sine;
    static Sample* triangle;
    static Sample* sawtooth;
    static Sample* square;
    
};

#endif
