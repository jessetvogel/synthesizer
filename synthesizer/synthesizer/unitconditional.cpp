#include "unitconditional.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "parameter.hpp"

UnitConditional::UnitConditional(Controller* controller, bool keyDependent) {
    // Store pointer to controller
    this->controller = controller;
    type = "conditional";
    
    // May or may not be key dependent
    this->keyDependent = keyDependent;
    
    // Set default values
    parameters.push_back(input = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "input", "0.0"));
    parameters.push_back(low = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "low", "0.0"));
    parameters.push_back(high = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "high", "0.0"));
    parameters.push_back(outputLow = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "output_low", "0.0"));
    parameters.push_back(outputMiddle = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "output_middle", "0.0"));
    parameters.push_back(outputHigh = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "output_high", "0.0"));
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
}

void UnitConditional::apply(Instrument* instrument) {
    Unit* input = (Unit*) (this->input->pointer);
    Unit* low = (Unit*) (this->low->pointer);
    Unit* high = (Unit*) (this->high->pointer);
    Unit* outputLow = (Unit*) (this->outputLow->pointer);
    Unit* outputMiddle = (Unit*) (this->outputMiddle->pointer);
    Unit* outputHigh = (Unit*) (this->outputHigh->pointer);
    
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x) {
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
