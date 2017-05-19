#include "unitconstant.hpp"
#include "controller.hpp"

UnitConstant::UnitConstant(Controller* controller, double value) {
    // Store pointer to controller
    this->controller = controller;
    
    // Store value
    this->value = value;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = value;
}

void UnitConstant::apply(Instrument* instrument) { }

bool UnitConstant::deleteOnRemove() { return true; }
