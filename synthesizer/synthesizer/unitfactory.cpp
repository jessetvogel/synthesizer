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
#include "unitmodulationwheel.hpp"
#include "unitaudioinput.hpp"
#include "unitaudiooutput.hpp"
#include "unitcollector.hpp"

#include "status.hpp"

void UnitFactory::createDefaultUnits(Controller* controller, Units* units) {
    units->addUnit((new UnitKeyInfo(controller, UnitKeyInfo::Frequency))->setId("key_frequency"));
    units->addUnit((new UnitKeyInfo(controller, UnitKeyInfo::Velocity))->setId("key_velocity"));
    units->addUnit((new UnitKeyInfo(controller, UnitKeyInfo::Duration))->setId("key_duration"));
    units->addUnit((new UnitKeyInfo(controller, UnitKeyInfo::Release))->setId("key_release"));
    
    units->addUnit((new UnitLeadKeyInfo(controller, UnitLeadKeyInfo::Frequency))->setId("lead_key_frequency"));
    units->addUnit((new UnitLeadKeyInfo(controller, UnitLeadKeyInfo::Velocity))->setId("lead_key_velocity"));
    units->addUnit((new UnitLeadKeyInfo(controller, UnitLeadKeyInfo::Duration))->setId("lead_key_duration"));
    units->addUnit((new UnitLeadKeyInfo(controller, UnitLeadKeyInfo::Release))->setId("lead_key_release"));
    units->addUnit((new UnitLeadKeyInfo(controller, UnitLeadKeyInfo::Pressing))->setId("lead_key_pressing"));
    
    units->addUnit((new UnitModulationWheel(controller))->setId("modulation_wheel"));
}

Unit* UnitFactory::create(Controller* controller, std::string type, std::string id, Arguments arguments) {
    Unit* unit = NULL;
    
    if(type.compare("adder") == 0)              unit = new UnitAdder(controller, arguments);
    if(type.compare("delay") == 0)              unit = new UnitDelay(controller, arguments);
    if(type.compare("lowpass") == 0)            unit = new UnitLowpass(controller, arguments);
    if(type.compare("highpass") == 0)           unit = new UnitHighpass(controller, arguments);
    if(type.compare("bandpass") == 0)           unit = new UnitBandpass(controller, arguments);
    if(type.compare("oscillator") == 0)         unit = new UnitOscillator(controller, arguments);
    if(type.compare("function") == 0)           unit = new UnitFunction(controller, arguments);
    if(type.compare("ADSR") == 0)               unit = new UnitADSR(controller, arguments);
    if(type.compare("conditional") == 0)        unit = new UnitConditional(controller, arguments);
    if(type.compare("fuzz") == 0)               unit = new UnitFuzz(controller, arguments);
    if(type.compare("PWM") == 0)                unit = new UnitPWM(controller, arguments);
    if(type.compare("volumemeter") == 0)        unit = new UnitVolumeMeter(controller, arguments);
    if(type.compare("label") == 0)              unit = new UnitLabel(controller, arguments);
    if(type.compare("parameter") == 0)          unit = new UnitParameter(controller, arguments);
    if(type.compare("collector") == 0)          unit = new UnitCollector(controller, arguments);
    if(type.compare("audio_input") == 0)        unit = new UnitAudioInput(controller, arguments);
    if(type.compare("audio_output") == 0)       unit = new UnitAudioOutput(controller, arguments);
    
    if(unit == NULL) {
        // If no match was found, return NULL
        Status::addError("Unit type does not exist");
        return NULL;
    }
    
    unit->setId(id);
    return unit;
}
