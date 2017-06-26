#ifndef IIRfilter_hpp
#define IIRfilter_hpp

class IIRFilter {

    int FFFO; // Feedforward filter order
    int FBFO; // Feedback filter order
    
    double* input;
    double* output;
    
public:
    
    IIRFilter(int, int);
    ~IIRFilter();
    
    double* alpha;
    double* beta;
    double gain;
    
    double apply(double);
    
};

#endif
