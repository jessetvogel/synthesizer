#include "keyfrequency.hpp"
#include "controller.hpp"
#include "instrument.hpp"

KeyFrequency::KeyFrequency(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Set default values
    // ...
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
}

KeyFrequency::~KeyFrequency() {
    delete[] output;
}

void KeyFrequency::apply(Instrument* instrument) {
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = instrument->currentFrequency;
}
