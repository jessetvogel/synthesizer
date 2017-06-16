#include <algorithm>

#include "unitconstant.hpp"
#include "controller.hpp"

UnitConstant::UnitConstant(Controller* controller, double value) {
    // Store pointer to controller
    this->controller = controller;
    type = "constant";
    
    // Store value
    this->value = value;
    
    // Constants are not key dependent
    keyDependent = false;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = value;
}
