#include "unitmodulationwheel.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "midistate.hpp"

UnitModulationWheel::UnitModulationWheel(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    type = "modulation_wheel";
    
    // The modulation wheel is not key dependent
    keyDependent = false;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
}

void UnitModulationWheel::apply(Instrument* instrument) {
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = controller->getMidiState()->modulationWheel;
}
