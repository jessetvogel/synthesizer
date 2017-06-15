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
#include "unitbandpass.hpp"
#include "unitvariable.hpp"
#include "unitconditional.hpp"
#include "unitfuzz.hpp"
#include "unitdelay.hpp"
#include "unitPWM.hpp"
#include "unitParameter.hpp"

#include "error.hpp"

Unit* Unit::create(Controller* controller, std::string type, bool keyDependent, std::string arg1, std::string arg2) {
    // Adder
    if(type.compare("adder") == 0) {
        if(arg2.length() != 0) { Error::addError(Error::INVALID_NUMBER_OF_ARGUMENTS); return NULL; } // Only requires one argument, not two
        if(!Util::isInteger(arg1)) { Error::addError(Error::INVALID_ARGUMENT); return NULL; }
        int n = stoi(arg1);
        if(n > UnitAdder::maxN) return NULL;
        return new UnitAdder(controller, keyDependent, n);
    }
    
    // Delay
    if(type.compare("delay") == 0) {
        if(keyDependent) return NULL;
        if(!Util::isInteger(arg1)) return NULL;
        if(!Util::isNumber(arg2)) return NULL;
        int n = stoi(arg1);
        double T = stod(arg2);
        if(n > UnitDelay::maxN) return NULL;
        if(T > UnitDelay::maxT) return NULL;
        return new UnitDelay(controller, n, T);
    }
    
    // Parameter
    if(type.compare("parameter") == 0) {
        if(arg2.length() != 0) return NULL; // Only requires one argument, not two
        if(keyDependent) return NULL;
        if(!Util::isInteger(arg1)) return NULL;
        int MidiCC = stoi(arg1);
        if(MidiCC < 0 || MidiCC > 127) return NULL;
        UnitParameter* parameter = new UnitParameter(controller, MidiCC);
        controller->addUnitParameter(parameter, MidiCC);
        return parameter;
    }
    
    // All types below have no arguments
    if(arg1.length() != 0) return NULL;
    
    // Variable
    if(type.compare("variable") == 0)
        return new UnitVariable(controller);
    
    // Oscillator
    if(type.compare("oscillator") == 0)
        return new UnitOscillator(controller, keyDependent);
    
    // Function
    if(type.compare("function") == 0)
        return new UnitFunction(controller, keyDependent);
    
    // ADSR envelope
    if(type.compare("ADSR") == 0)
        return new UnitADSR(controller, keyDependent);
    
    // Conditional
    if(type.compare("conditional") == 0)
        return new UnitConditional(controller, keyDependent);
    
    // Fuzz
    if(type.compare("fuzz") == 0)
        return new UnitFuzz(controller, keyDependent);
    
    // PWM
    if(type.compare("PWM") == 0)
        return new UnitPWM(controller, keyDependent);

    // Types below are not allowed to be key dependent
    if(keyDependent) return NULL;
    
    // Low-pass filter
    if(type.compare("lowpass") == 0)
        return new UnitLowpass(controller);
    
    // High-pass filter
    if(type.compare("highpass") == 0)
        return new UnitHighpass(controller);
    
    // Band-pass filter
    if(type.compare("bandpass") == 0)
        return new UnitBandPass(controller);
    
    // If no match was found, return NULL
    Error::addError(Error::UNIT_TYPE_NOT_FOUND);
    return NULL;
}

bool Unit::set(Controller* controller, Unit** parameterAddr, std::string value, bool allowKeyDependent) {
    if(Util::isNumber(value)) {
        // If we are overwriting a unit constant, delete the old constant
        if(*parameterAddr != NULL && controller->isUnitConstant(*parameterAddr))
            controller->deleteUnitConstant((UnitConstant*) *parameterAddr);
        
        // Create a new unit constant
        *parameterAddr = controller->createUnitConstant(stod(value));
        return true;
    }
    else {
        // Check if given unit exists
        Unit* unit = controller->getUnit(value);
        if(unit == NULL) { Error::addError(Error::UNIT_NOT_FOUND); return false; }
        
        // Check for key dependence
        if(!allowKeyDependent && unit->keyDependent) { Error::addError(Error::EXPECTED_KEY_UNDEPENDENT); return false; }
        
        // If we are overwriting a unit constant, delete the old constant
        if(*parameterAddr != NULL && controller->isUnitConstant(*parameterAddr))
            controller->deleteUnitConstant((UnitConstant*) *parameterAddr);
        
        *parameterAddr = unit;
        return true;
    }
}
