#include "unitconditional.hpp"
#include "controller.hpp"
#include "instrument.hpp"

UnitConditional::UnitConditional(Controller* controller, bool keyDependent) {
    // Store pointer to controller
    this->controller = controller;
    
    // May or may not be key dependent
    this->keyDependent = keyDependent;
    
    // Set default values
    Unit::set(controller, &input, "0.0", keyDependent);
    Unit::set(controller, &low, "0.0", keyDependent);
    Unit::set(controller, &high, "0.0", keyDependent);
    Unit::set(controller, &outputLow, "0.0", keyDependent);
    Unit::set(controller, &outputMiddle, "0.0", keyDependent);
    Unit::set(controller, &outputHigh, "0.0", keyDependent);
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
}

UnitConditional::~UnitConditional() {
    delete[] output;
}

void UnitConditional::apply(Instrument* instrument) {
    input->update(instrument);
    low->update(instrument);
    high->update(instrument);
    outputLow->update(instrument);
    outputMiddle->update(instrument);
    outputHigh->update(instrument);
    
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

bool UnitConditional::setValue(std::string parameter, std::string value) {
    if(parameter.compare("input") == 0)
        return Unit::set(controller, &input, value, keyDependent);
    
    if(parameter.compare("low") == 0)
        return Unit::set(controller, &low, value, keyDependent);
    
    if(parameter.compare("high") == 0)
        return Unit::set(controller, &high, value, keyDependent);
    
    if(parameter.compare("output_low") == 0)
        return Unit::set(controller, &outputLow, value, keyDependent);
    
    if(parameter.compare("output_middle") == 0)
        return Unit::set(controller, &outputMiddle, value, keyDependent);
    
    if(parameter.compare("output_high") == 0)
        return Unit::set(controller, &outputHigh, value, keyDependent);
    
    return false;
};
