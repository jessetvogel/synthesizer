#include "unitlabel.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "parameter.hpp"

UnitLabel::UnitLabel(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    type = "label";
    
    // Obviously not key dependent
    keyDependent = false;
    
    // Default value is zero
    parameters.push_back(value = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "value", "0.0"));
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
}

void UnitLabel::apply(Instrument* instrument) {
    Unit* value = (Unit*) (this->value->pointer);
    
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x) {
        output[x] = value->output[x];
    }
}
