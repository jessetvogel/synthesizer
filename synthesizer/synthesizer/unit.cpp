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

bool Unit::setValue(std::string parameter, std::string value) { return false; };

void Unit::reset() { updated = false; };

void Unit::update(Instrument* instrument) { if(updated) return; if(!applyAlways) updated = true; apply(instrument); };

void Unit::apply(Instrument* instrument) { }

bool Unit::isKeyDependent() { return keyDependent; }

Unit* Unit::create(Controller* controller, std::string type, bool keyDependent) {

    // Oscillator
    if(type.compare("oscillator") == 0)
        return new UnitOscillator(controller, keyDependent);
    
    // Adder of size n in {1, 2, ... , UnitAdder::maxN}
    for(int n = 1;n <= UnitAdder::maxN;n ++) {
        char typeName[12];
        sprintf(typeName, "adder_%d", n);
        if(type.compare(typeName) == 0)
            return new UnitAdder(controller, keyDependent, n);
    }
    
    // Function
    if(type.compare("function") == 0)
        return new UnitFunction(controller, keyDependent);
    
    // ADSR envelope (requires to be keyDependent)
    if(type.compare("ADSR") == 0 && keyDependent)
        return new UnitADSR(controller);
    
    // Low-pass filter
    if(type.compare("lowpass") == 0)
        return new UnitLowpass(controller, keyDependent);
    
    // High-pass filter
    if(type.compare("highpass") == 0)
        return new UnitHighpass(controller, keyDependent);

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
