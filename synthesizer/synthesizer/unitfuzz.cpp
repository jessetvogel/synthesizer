#include "unitfuzz.hpp"
#include "controller.hpp"
#include "parameter.hpp"
#include "arguments.hpp"
#include "function.hpp"

UnitFuzz::UnitFuzz(Controller* controller, Arguments arguments) : Unit(controller) {
    // Set type
    type = "fuzz";
    
    // Set arguments
    keyDependent = arguments.getBool("key", false);
    
    // Set parameters
    parameters.push_back(input = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "input", "0.0"));
    parameters.push_back(inputGain = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "input_gain", "1.0"));
}

void UnitFuzz::apply() {
    Unit* input = (Unit*) (this->input->pointer);
    Unit* inputGain = (Unit*) (this->inputGain->pointer);
    
    for(int x = 0;x < framesPerBuffer; ++x)
        output[x] = Function::fuzz(input->output[x], inputGain->output[x]);
}
