#include "unitinput.hpp"
#include "controller.hpp"
#include "instrument.hpp"

UnitInput::UnitInput(Controller* controller) : Unit(controller) {
    // Set type
    type = "input";
    
    // Obviously not key dependent
    keyDependent = false;
}

void UnitInput::apply(Instrument* instrument) {
    // Copy input signal
    float* bufferInput = controller->getBufferInput();
    for(int x = 0;x < framesPerBuffer; ++x) {
        output[x] = bufferInput[x];
    }
}
