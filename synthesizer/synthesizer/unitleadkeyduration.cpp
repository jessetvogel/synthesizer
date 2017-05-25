#include "unitleadkeyduration.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "midistate.hpp"

UnitLeadKeyDuration::UnitLeadKeyDuration(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Not key dependent
    keyDependent = false;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
}

UnitLeadKeyDuration::~UnitLeadKeyDuration() {
    delete[] output;
}

void UnitLeadKeyDuration::apply(Instrument* instrument) {
    double duration = controller->getMidiState()->leadKey.duration;
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = duration;
}
