#ifndef unitfunction_hpp
#define unitfunction_hpp

#include "unit.hpp"
#include "function.hpp"

class UnitFunction : public Unit {
    
    Unit* input = NULL;
    Function::Type function;
    
public:
    
    UnitFunction(Controller*, bool);
    ~UnitFunction();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
    
};

#endif
