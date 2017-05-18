#ifndef filter_hpp
#define filter_hpp

class Filter {

    double low;
    double high;
    int bands;
    double* values;
    
    double logConstant;
    
public:
    
    Filter(double, double, int, double*);
    ~Filter();
    double at(double);
    
};

#endif
