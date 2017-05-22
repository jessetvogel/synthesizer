#ifndef unitvariable_hpp
#define unitvariable_hpp

#include "unit.hpp"

class UnitVariable : public Unit {
    
    double value;
    
public:
    
    UnitVariable(Controller*);
    ~UnitVariable();
    
    inline void apply(Instrument*) {};
    bool setValue(std::string, std::string);
};

#endif
