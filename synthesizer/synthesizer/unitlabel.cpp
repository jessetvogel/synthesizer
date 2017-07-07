#include "unitlabel.hpp"
#include "controller.hpp"
#include "parameter.hpp"
#include "arguments.hpp"

UnitLabel::UnitLabel(Controller* controller, Arguments) : Unit(controller) {
    // Set type
    type = "label";
    
    // Default value is zero
    parameters.push_back(value = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "value", "0.0"));
}

void UnitLabel::apply() {
    Unit* value = (Unit*) (this->value->pointer);
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        output[x] = value->output[x];
    }
}
