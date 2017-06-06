#include "unitvariable.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "util.hpp"

UnitVariable::UnitVariable(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Obviously not key dependent
    keyDependent = false;
    
    // Default value is zero
    Unit::set(controller, &value, "0.0", keyDependent);
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
}

UnitVariable::~UnitVariable() {
    delete[] output;
}

void UnitVariable::apply(Instrument* instrument) {
    value->update(instrument);
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x) {
        output[x] = value->output[x];
    }
}

bool UnitVariable::setValue(std::string parameter, std::string value) {
    if(parameter.compare("value") == 0)
        return Unit::set(controller, &(this->value), value, keyDependent);
    
    return false;
}
