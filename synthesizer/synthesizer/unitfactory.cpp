#include "unitfactory.hpp"

#include "controller.hpp"
#include "units.hpp"
#include "arguments.hpp"

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
#include "unitkeyinfo.hpp"
#include "unitleadkeyinfo.hpp"
#include "unitkeyoutput.hpp"
#include "unitmodulationwheel.hpp"
#include "unitinput.hpp"

#include "error.hpp"

void UnitFactory::createDefaultUnits(Controller* controller, Units* units) {
    units->add((new UnitKeyInfo(controller, UnitKeyInfo::Frequency))->setId("key_frequency"));
    units->add((new UnitKeyInfo(controller, UnitKeyInfo::Velocity))->setId("key_velocity"));
    units->add((new UnitKeyInfo(controller, UnitKeyInfo::Duration))->setId("key_duration"));
    units->add((new UnitKeyInfo(controller, UnitKeyInfo::Release))->setId("key_release"));
    
    units->add((new UnitLeadKeyInfo(controller, UnitLeadKeyInfo::Frequency))->setId("lead_key_frequency"));
    units->add((new UnitLeadKeyInfo(controller, UnitLeadKeyInfo::Velocity))->setId("lead_key_velocity"));
    units->add((new UnitLeadKeyInfo(controller, UnitLeadKeyInfo::Duration))->setId("lead_key_duration"));
    units->add((new UnitLeadKeyInfo(controller, UnitLeadKeyInfo::Release))->setId("lead_key_release"));
    units->add((new UnitLeadKeyInfo(controller, UnitLeadKeyInfo::Pressing))->setId("lead_key_pressing"));
    
    units->add((new UnitKeyOutput(controller))->setId("key_output"));
    units->add((new UnitModulationWheel(controller))->setId("modulation_wheel"));
    
    units->add((new UnitInput(controller))->setId("audio_input"));
}

Unit* UnitFactory::create(Controller* controller, std::string type, std::string id, Arguments arguments) {
    Unit* unit = NULL;
    
    if(type.compare("adder") == 0) unit = new UnitAdder(controller, arguments);
    
//    {
//        if(arg2.length() != 0) { Error::addError(Error::INVALID_NUMBER_OF_ARGUMENTS); return NULL; } // Only requires one argument, not two
//        if(!Util::isInteger(arg1)) { Error::addError(Error::INVALID_ARGUMENT); return NULL; }
//        int n = stoi(arg1);
//        if(n > UnitAdder::maxN) return NULL;
//        return new UnitAdder(controller, keyDependent, n);
//    }
    
    if(type.compare("delay") == 0) unit = new UnitDelay(controller, arguments);

//    {
//        if(keyDependent) return NULL;
//        if(!Util::isInteger(arg1)) return NULL;
//        if(!Util::isNumber(arg2)) return NULL;
//        int n = stoi(arg1);
//        double T = stod(arg2);
//        if(n > UnitDelay::maxN) return NULL;
//        if(T > UnitDelay::maxT) return NULL;
//        return new UnitDelay(controller, n, T);
//    }
    
    if(type.compare("lowpass") == 0) unit = new UnitLowpass(controller, arguments);
    
//    {
//        if(keyDependent) return NULL;
//        if(arg2.length() != 0) { Error::addError(Error::INVALID_NUMBER_OF_ARGUMENTS); return NULL; } // Only requires one argument, not two
//        if(!Util::isInteger(arg1)) { Error::addError(Error::INVALID_ARGUMENT); return NULL; }
//        int order = stoi(arg1);
//        if(order > UnitLowpass::maxOrder) return NULL;
//        return new UnitLowpass(controller, order);
//    }
    
    if(type.compare("highpass") == 0) unit = new UnitHighpass(controller, arguments);
    
//    {
//        if(keyDependent) return NULL;
//        if(arg2.length() != 0) { Error::addError(Error::INVALID_NUMBER_OF_ARGUMENTS); return NULL; } // Only requires one argument, not two
//        if(!Util::isInteger(arg1)) { Error::addError(Error::INVALID_ARGUMENT); return NULL; }
//        int order = stoi(arg1);
//        if(order > UnitHighpass::maxOrder) return NULL;
//        return new UnitHighpass(controller, order);
//    }
    
    if(type.compare("bandpass") == 0) unit = new UnitBandpass(controller, arguments);
    
//    {
//        if(keyDependent) return NULL;
//        if(arg2.length() != 0) { Error::addError(Error::INVALID_NUMBER_OF_ARGUMENTS); return NULL; } // Only requires one argument, not two
//        if(!Util::isInteger(arg1)) { Error::addError(Error::INVALID_ARGUMENT); return NULL; }
//        int order = stoi(arg1);
//        if(order > UnitBandpass::maxOrder) return NULL;
//        return new UnitBandpass(controller, order);
//    }
    
    if(type.compare("parameter") == 0) unit = new UnitParameter(controller, arguments);
    
//    {
//        if(arg2.length() != 0) return NULL; // Only requires one argument, not two
//        if(keyDependent) return NULL;
//        if(!Util::isInteger(arg1)) return NULL;
//        int MidiCC = stoi(arg1);
//        if(MidiCC < 0 || MidiCC > 127) return NULL;
//        UnitParameter* parameter = new UnitParameter(controller, MidiCC);
//        controller->getUnits()->addParameter(parameter, MidiCC);
//        return parameter;
//    }
    

    if(type.compare("oscillator") == 0)         unit = new UnitOscillator(controller, arguments);
    if(type.compare("function") == 0)           unit = new UnitFunction(controller, arguments);
    if(type.compare("ADSR") == 0)               unit = new UnitADSR(controller, arguments);
    if(type.compare("conditional") == 0)        unit = new UnitConditional(controller, arguments);
    if(type.compare("fuzz") == 0)               unit = new UnitFuzz(controller, arguments);
    if(type.compare("PWM") == 0)                unit = new UnitPWM(controller, arguments);
    if(type.compare("volumemeter") == 0)        unit = new UnitVolumeMeter(controller, arguments);
    if(type.compare("label") == 0)              unit = new UnitLabel(controller, arguments);
    
    if(unit == NULL) {
        // If no match was found, return NULL
        Error::addError(Error::UNIT_TYPE_NOT_FOUND);
        return NULL;
    }
    
    unit->setId(id);
    return unit;
}
