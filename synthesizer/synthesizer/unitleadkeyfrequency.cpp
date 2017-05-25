#include "unitleadkeyfrequency.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "midistate.hpp"

UnitLeadKeyFrequency::UnitLeadKeyFrequency(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Not key dependent
    keyDependent = false;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
}

UnitLeadKeyFrequency::~UnitLeadKeyFrequency() {
    delete[] output;
}

void UnitLeadKeyFrequency::apply(Instrument* instrument) {
    double frequency = controller->getMidiState()->leadKey.frequency;
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = frequency;
}
