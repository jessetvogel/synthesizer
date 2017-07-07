#include "unitmodulationwheel.hpp"
#include "controller.hpp"
#include "midistate.hpp"

UnitModulationWheel::UnitModulationWheel(Controller* controller) : Unit(controller) {
    // Set type
    type = "modulation_wheel";
    
    // Hidden
    hidden = true;
}

void UnitModulationWheel::apply() {
    double value = controller->getMidiState()->modulationWheel;
    for(int x = 0;x < framesPerBuffer; ++x)
        output[x] = value;
}
