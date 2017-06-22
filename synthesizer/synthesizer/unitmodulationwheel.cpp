#include "unitmodulationwheel.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "midistate.hpp"

UnitModulationWheel::UnitModulationWheel(Controller* controller) : Unit(controller) {
    // Set type
    type = "modulation_wheel";
    
    // Hidden
    hidden = true;
    
    // The modulation wheel is not key dependent
    keyDependent = false;
}

void UnitModulationWheel::apply(Instrument* instrument) {
    double value = controller->getMidiState()->modulationWheel;
    for(int x = 0;x < framesPerBuffer; ++x)
        output[x] = value;
}
