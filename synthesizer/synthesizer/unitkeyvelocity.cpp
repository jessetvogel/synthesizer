#include "unitkeyvelocity.hpp"
#include "controller.hpp"
#include "instrument.hpp"

UnitKeyVelocity::UnitKeyVelocity(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Obviously key dependent
    keyDependent = true;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
}

UnitKeyVelocity::~UnitKeyVelocity() {
    delete[] output;
}

void UnitKeyVelocity::apply(Instrument* instrument) {
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = instrument->currentVelocity;
}
