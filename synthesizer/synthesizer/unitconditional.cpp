#include "unitconditional.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "parameter.hpp"
#include "arguments.hpp"

UnitConditional::UnitConditional(Controller* controller, Arguments arguments) : Unit(controller) {
    // Set type
    type = "conditional";
    
    // Set arguments
    keyDependent = arguments.getBool("key", false);
    
    // Set parameters
    parameters.push_back(input = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "input", "0.0"));
    parameters.push_back(low = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "low", "0.0"));
    parameters.push_back(high = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "high", "0.0"));
    parameters.push_back(outputLow = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "output_low", "0.0"));
    parameters.push_back(outputMiddle = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "output_middle", "0.0"));
    parameters.push_back(outputHigh = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "output_high", "0.0"));
}

void UnitConditional::apply(Instrument* instrument) {
    Unit* input = (Unit*) (this->input->pointer);
    Unit* low = (Unit*) (this->low->pointer);
    Unit* high = (Unit*) (this->high->pointer);
    Unit* outputLow = (Unit*) (this->outputLow->pointer);
    Unit* outputMiddle = (Unit*) (this->outputMiddle->pointer);
    Unit* outputHigh = (Unit*) (this->outputHigh->pointer);
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        float y = input->output[x];
        if(y < low->output[x]) {
            output[x] = outputLow->output[x];
            continue;
        }
        if(y > high->output[x]) {
            output[x] = outputHigh->output[x];
            continue;
        }
        output[x] = outputMiddle->output[x];
    }
}
