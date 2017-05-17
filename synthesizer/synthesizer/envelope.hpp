#ifndef envelope_hpp
#define envelope_hpp

class Envelope {

public:
    
    bool finished;
    
    virtual float value(bool, double, double);
    
    static void initialize();
    static Envelope* standard;
    
};

#endif
