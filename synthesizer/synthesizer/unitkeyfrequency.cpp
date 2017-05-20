#include "unitkeyfrequency.hpp"
#include "controller.hpp"
#include "instrument.hpp"

UnitKeyFrequency::UnitKeyFrequency(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Obviously key dependent
    keyDependent = true;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
}

UnitKeyFrequency::~UnitKeyFrequency() {
    delete[] output;
}

void UnitKeyFrequency::apply(Instrument* instrument) {
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = instrument->currentFrequency;
}
