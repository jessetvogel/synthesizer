#include <cstdlib>

#include "units.hpp"
#include "unit.hpp"
#include "util.hpp"
#include "controller.hpp"
#include "settings.hpp"
#include "parameter.hpp"

#include "unitoscillator.hpp"
#include "unitconstant.hpp"
#include "unitadder.hpp"
#include "unitfunction.hpp"
#include "unitADSR.hpp"
#include "unitlowpass.hpp"
#include "unithighpass.hpp"
#include "unitbandpass.hpp"
#include "unitlabel.hpp"
#include "unitconditional.hpp"
#include "unitfuzz.hpp"
#include "unitdelay.hpp"
#include "unitPWM.hpp"
#include "unitparameter.hpp"
#include "unitvolumemeter.hpp"

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
        controller->getUnits()->addParameter(parameter, MidiCC);
        return parameter;
    }
    
    // All types below have no arguments
    if(arg1.length() != 0) return NULL;
    
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

    // Volume meter
    if(type.compare("volumemeter") == 0)
        return new UnitVolumeMeter(controller, keyDependent);
    
    // Types below are not allowed to be key dependent
    if(keyDependent) return NULL;
    
    // Label
    if(type.compare("label") == 0)
        return new UnitLabel(controller);
    
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

Unit::Unit(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Store some variables
    framesPerBuffer = controller->getSettings()->bufferSize;
    sampleRate = controller->getSettings()->sampleRate;
    
    // Create output array
    output = new float[framesPerBuffer];
    memset(output, 0, sizeof(float) * framesPerBuffer);
}

Unit::~Unit() {
    delete[] output;
    
    for(auto it = parameters.begin(); it != parameters.end(); ++it)
        delete *it;
}

Unit* Unit::setId(std::string id) {
    //    // Only allowed to change id when not yet set
    //    if(this->id.compare(UNIT_DEFAULT_ID) != 0) {
    //        return false;
    //    }
    this->id = id;
    return this;
}

void Unit::update(Instrument* instrument) {
    if(updated)
        return;
    
    if(!applyAlways)
        updated = true;
    
    // Update all unit parameters
    for(auto it = parameters.begin(); it != parameters.end(); ++it) {
        if((*it)->type == Parameter::UNIT || (*it)->type == Parameter::UNIT_KEY_INDEPENDENT)
            ((Unit*) ((*it)->pointer))->update(instrument);
    }
    
    apply(instrument);
};

bool Unit::setParameter(std::string label, std::string value) {
    for(auto it = parameters.begin(); it != parameters.end(); ++it) {
        if(label.compare((*it)->label) == 0)
            return (*it)->set(value);
    }
    Error::addError(Error::UNKNOWN_PARAMETER);
    return false;
}

