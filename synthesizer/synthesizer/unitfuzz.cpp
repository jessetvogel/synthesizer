#include "unitfuzz.hpp"
#include "controller.hpp"
#include "parameter.hpp"
#include "function.hpp"

UnitFuzz::UnitFuzz(Controller* controller, bool keyDependent) : Unit(controller) {
    // Set type
    type = "fuzz";
    
    // May or may not be key dependent
    this->keyDependent = keyDependent;
    
    // Set default values
    parameters.push_back(input = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "input", "0.0"));
    parameters.push_back(inputGain = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "input_gain", "1.0"));
}

void UnitFuzz::apply(Instrument* instrument) {
    Unit* input = (Unit*) (this->input->pointer);
    Unit* inputGain = (Unit*) (this->inputGain->pointer);
    
    for(int x = 0;x < framesPerBuffer; ++x)
        output[x] = Function::fuzz(input->output[x], inputGain->output[x]);
}
