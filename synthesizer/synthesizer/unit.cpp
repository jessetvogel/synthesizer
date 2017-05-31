#include <cstdlib>

#include "unit.hpp"
#include "util.hpp"
#include "controller.hpp"

#include "unitoscillator.hpp"
#include "unitconstant.hpp"
#include "unitadder.hpp"
#include "unitfunction.hpp"
#include "unitADSR.hpp"
#include "unitlowpass.hpp"
#include "unithighpass.hpp"
#include "unitvariable.hpp"
#include "unitconditional.hpp"
#include "unitfuzz.hpp"

Unit* Unit::create(Controller* controller, std::string type, bool keyDependent) {

    // Variable
    if(type.compare("variable") == 0)
        return new UnitVariable(controller);
    
    // Adder of size n in {1, 2, ... , UnitAdder::maxN}
    for(int n = 1;n <= UnitAdder::maxN;n ++) {
        char typeName[12];
        sprintf(typeName, "adder_%d", n);
        if(type.compare(typeName) == 0)
            return new UnitAdder(controller, keyDependent, n);
    }
    
    // Oscillator
    if(type.compare("oscillator") == 0)
        return new UnitOscillator(controller, keyDependent);
    
    // Function
    if(type.compare("function") == 0)
        return new UnitFunction(controller, keyDependent);
    
    // ADSR envelope
    if(type.compare("ADSR") == 0)
        return new UnitADSR(controller, keyDependent);
    
    // Low-pass filter
    if(type.compare("lowpass") == 0)
        return new UnitLowpass(controller, keyDependent);
    
    // High-pass filter
    if(type.compare("highpass") == 0)
        return new UnitHighpass(controller, keyDependent);
    
    // Conditional
    if(type.compare("conditional") == 0)
        return new UnitConditional(controller, keyDependent);
    
    // Fuzz
    if(type.compare("fuzz") == 0)
        return new UnitFuzz(controller, keyDependent);

    // If no match was found, return NULL
    return NULL;
}

bool Unit::set(Controller* controller, Unit** parameterAddr, std::string value, bool allowKeyDependent) {
    if(Util::isNumber(value)) {
        if(*parameterAddr != NULL && (*parameterAddr)->constant) delete (*parameterAddr);
        *parameterAddr = new UnitConstant(controller, stod(value));
        (*parameterAddr)->constant = true;
        return true;
    }
    else {
        Unit* unit = controller->getUnit(value);
        if(unit == NULL) return false;
        if(!allowKeyDependent && unit->keyDependent) return false;
        
        if(*parameterAddr != NULL && (*parameterAddr)->constant) delete (*parameterAddr);
        *parameterAddr = unit;
        return true;
    }
}
