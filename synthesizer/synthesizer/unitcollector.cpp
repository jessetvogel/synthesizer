#include "unitcollector.hpp"
#include "controller.hpp"
#include "midistate.hpp"
#include "units.hpp"
#include "parameter.hpp"
#include "arguments.hpp"

UnitCollector::UnitCollector(Controller* controller, Arguments arguments) : Unit(controller) {
    // Set type
    type = "collector";
    
    // Should not update parameters (as this is done by controller->units)
    updateParameters = false;
    
    // Set parameters
    parameters.push_back(input = new Parameter(controller, Parameter::UNIT, "input", "0.0"));
    parameters.push_back(releaseTime = new Parameter(controller, Parameter::UNIT_KEY_INDEPENDENT, "release_time", "0.0"));
}

void UnitCollector::addKeyEvent(KeyEvent* keyEvent) {
    keyEvents.push_back(keyEvent);
}

double UnitCollector::getReleaseTime() {
    return ((Unit*) (releaseTime->pointer))->output[0];
}

Unit* UnitCollector::getInput() {
    return (Unit*) (input->pointer);
}
