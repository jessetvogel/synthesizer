#include "unitfunction.hpp"
#include "controller.hpp"
#include "parameter.hpp"
#include "arguments.hpp"

UnitFunction::UnitFunction(Controller* controller, Arguments arguments) : Unit(controller) {
    // Set type
    type = "function";
    
    // Set arguments
    keyDependent = arguments.getBool("key", false);
    
    // Set parameters
    parameters.push_back(input = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "input", "0.0"));
    parameters.push_back(function = new Parameter(controller, Parameter::FUNCTION, "function", "identity"));
}

void UnitFunction::apply() {
    Unit* input = (Unit*) (this->input->pointer);
    Function* function = (Function*) (this->function->pointer);
    
    for(int x = 0;x < framesPerBuffer; ++x)
        output[x] = Function::evaluate(function, input->output[x]);
}
