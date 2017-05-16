#ifndef sample_hpp
#define sample_hpp

class Sample {

    int resolution;
    float* data;
    
public:
    
    Sample(int, float*);
    ~Sample();
    
    Sample(Sample*, Sample*, float);
    
    float getValue(double);
    
};

#endif
