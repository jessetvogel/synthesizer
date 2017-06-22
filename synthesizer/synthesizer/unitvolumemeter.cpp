#include <cmath>

#include "unitvolumemeter.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "parameter.hpp"

UnitVolumeMeter::UnitVolumeMeter(Controller* controller, bool keyDependent) : Unit(controller) {
    // Set type
    type = "volumemeter";
    
    // May or may not be key dependent
    this->keyDependent = keyDependent;
    
    // Set default values
    parameters.push_back(input = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "input", "0.0"));
    RMSAverage = 0.0;
}

void UnitVolumeMeter::apply(Instrument* instrument) {
    Unit* input = (Unit*) (this->input->pointer);
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        RMSAverage = 0.995f * RMSAverage + 0.005f * (input->output[x]*input->output[x]);
        output[x] = std::sqrt(RMSAverage);
    }
}
