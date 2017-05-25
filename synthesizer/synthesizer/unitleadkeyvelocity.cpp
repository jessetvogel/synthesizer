#include "unitleadkeyvelocity.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "midistate.hpp"

UnitLeadKeyVelocity::UnitLeadKeyVelocity(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Not key dependent
    keyDependent = false;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
}

UnitLeadKeyVelocity::~UnitLeadKeyVelocity() {
    delete[] output;
}

void UnitLeadKeyVelocity::apply(Instrument* instrument) {
    double velocity = controller->getMidiState()->leadKey.velocity;
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = velocity;
}
