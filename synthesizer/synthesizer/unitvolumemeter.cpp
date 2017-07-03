#include <cmath>

#include "unitvolumemeter.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "parameter.hpp"
#include "arguments.hpp"

UnitVolumeMeter::UnitVolumeMeter(Controller* controller, Arguments arguments) : Unit(controller) {
    // Set type
    type = "volumemeter";
    
    // Set arguments
    keyDependent = arguments.getBool("key", false);
    
    // Set parameters
    parameters.push_back(input = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "input", "0.0"));
    RMSAverage = 0.0;
}

void UnitVolumeMeter::apply(Instrument* instrument) {
    Unit* input = (Unit*) (this->input->pointer);
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        RMSAverage = 0.998f * RMSAverage + 0.002f * (input->output[x]*input->output[x]); // TODO: coefficients?
        output[x] = std::sqrt(RMSAverage);
    }
}
