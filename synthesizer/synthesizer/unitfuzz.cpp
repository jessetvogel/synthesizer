#include "unitfuzz.hpp"
#include "controller.hpp"
#include "function.hpp"

UnitFuzz::UnitFuzz(Controller* controller, bool keyDependent) {
    // Store pointer to controller
    this->controller = controller;
    
    // May or may not be key dependent
    this->keyDependent = keyDependent;
    
    // Set default values
    Unit::set(controller, &input, "0.0", keyDependent);
    Unit::set(controller, &inputGain, "1.0", keyDependent);
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
}

UnitFuzz::~UnitFuzz() {
    delete[] output;
}

void UnitFuzz::apply(Instrument* instrument) {
    input->update(instrument);
    inputGain->update(instrument);
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = Function::fuzz(input->output[x], inputGain->output[x]);
}

bool UnitFuzz::setValue(std::string parameter, std::string value) {
    if(parameter.compare("input") == 0)
        return Unit::set(controller, &input, value, keyDependent);
    
    if(parameter.compare("input_gain") == 0)
        return Unit::set(controller, &inputGain, value, keyDependent);
    
    return false;
}
