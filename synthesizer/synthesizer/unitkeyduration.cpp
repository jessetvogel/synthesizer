#include "unitkeyduration.hpp"
#include "controller.hpp"
#include "instrument.hpp"

UnitKeyDuration::UnitKeyDuration(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Obviously key dependent
    keyDependent = true;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
}

UnitKeyDuration::~UnitKeyDuration() {
    delete[] output;
}

void UnitKeyDuration::apply(Instrument* instrument) {
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = instrument->currentKey->duration;
}
