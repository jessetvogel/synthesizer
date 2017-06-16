#include "unitfunction.hpp"
#include "controller.hpp"
#include "parameter.hpp"

UnitFunction::UnitFunction(Controller* controller, bool keyDependent) {
    // Store pointer to controller
    this->controller = controller;
    type = "function";
    
    // May or may not be key dependent
    this->keyDependent = keyDependent;
    
    // Set default values
    parameters.push_back(input = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "input", "0.0"));
    parameters.push_back(function = new Parameter(controller, Parameter::FUNCTION, "function", "identity"));
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
}

void UnitFunction::apply(Instrument* instrument) {
    Unit* input = (Unit*) (this->input->pointer);
    Function* function = (Function*) (this->function->pointer);
    
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = Function::evaluate(function, input->output[x]);
}
