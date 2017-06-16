#include "unitkeyoutput.hpp"
#include "controller.hpp"
#include "instrument.hpp"

UnitKeyOutput::UnitKeyOutput(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    type = "key_output";
    
    // Not key dependent
    keyDependent = false;
    applyAlways = true;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
}

void UnitKeyOutput::apply(Instrument* instrument) {
    float* keyBuffer = instrument->getKeyBuffer();
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = keyBuffer[x];
}
