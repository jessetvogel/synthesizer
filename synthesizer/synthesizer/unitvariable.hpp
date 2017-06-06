#ifndef unitvariable_hpp
#define unitvariable_hpp

#include "unit.hpp"

class UnitVariable : public Unit {
    
    Unit* value = NULL;
    
public:
    
    UnitVariable(Controller*);
    ~UnitVariable();
    
    void apply(Instrument*);
    bool setValue(std::string, std::string);
};

#endif
