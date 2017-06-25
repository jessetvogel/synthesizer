#ifndef IIRfilter_hpp
#define IIRfilter_hpp

class IIRFilter {

    int FFFO; // Feedforward filter order
    int FBFO; // Feedback filter order
    
    float* input;
    float* output;
    
public:
    
    IIRFilter(int, int);
    ~IIRFilter();
    
    float* alpha;
    float* beta;
    float gain;
    
    float apply(float);
    
};

#endif
