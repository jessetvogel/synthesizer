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
    value = 0.0;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
}

UnitVariable::~UnitVariable() {
    delete[] output;
}

bool UnitVariable::setValue(std::string parameter, std::string value) {
    if(parameter.compare("value") == 0) {
        if(!Util::isNumber(value))
            return false;
        this->value = stod(value);
        for(int x = 0;x < controller->getFramesPerBuffer(); ++x) output[x] = this->value;
        return true;
    }
    
    return false;
}
