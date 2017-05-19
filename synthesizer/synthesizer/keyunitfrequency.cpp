#include "keyunitfrequency.hpp"
#include "controller.hpp"
#include "instrument.hpp"

KeyUnitFrequency::KeyUnitFrequency(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
}

KeyUnitFrequency::~KeyUnitFrequency() {
    delete[] output;
}

void KeyUnitFrequency::apply(Instrument* instrument) {
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = instrument->currentFrequency;
}
