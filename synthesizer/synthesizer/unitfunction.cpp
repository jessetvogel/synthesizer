#include "unitfunction.hpp"
#include "controller.hpp"

UnitFunction::UnitFunction(Controller* controller, bool keyDependent) {
    // Store pointer to controller
    this->controller = controller;
    
    // May or may not be key dependent
    this->keyDependent = keyDependent;
    
    // Set default values
    Unit::set(controller, &input, "0.0", keyDependent);
    function = Function::Identity;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
}

UnitFunction::~UnitFunction() {
    delete[] output;
}

void UnitFunction::apply(Instrument* instrument) {
    input->update(instrument);
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = Function::evaluate(function, input->output[x]);
}

bool UnitFunction::setValue(std::string parameter, std::string value) {
    if(parameter.compare("input") == 0)
        return Unit::set(controller, &input, value, keyDependent);
    
    if(parameter.compare("function") == 0)
        return Function::set(controller, &function, value);
    
    return false;
}
