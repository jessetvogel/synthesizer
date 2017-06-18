#include "unitkeyoutput.hpp"
#include "controller.hpp"
#include "instrument.hpp"

UnitKeyOutput::UnitKeyOutput(Controller* controller) : Unit(controller) {
    // Set type
    type = "key_output";
    
    // Not key dependent
    keyDependent = false;
    applyAlways = true;
}

void UnitKeyOutput::apply(Instrument* instrument) {
    float* keyBuffer = instrument->getKeyBuffer();
    for(int x = 0;x < framesPerBuffer; ++x)
        output[x] = keyBuffer[x];
}
