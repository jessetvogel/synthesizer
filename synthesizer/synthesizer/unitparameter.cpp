#include "unitparameter.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "util.hpp"

UnitParameter::UnitParameter(Controller* controller, int MidiCCNumber) {
    // Store pointer to controller and other information
    this->controller = controller;
    this->MidiCCNumber = MidiCCNumber;
    
    // Not key dependent
    keyDependent = false;
    
    // Set default values
    value = 0.0;
    curve = Interpolation::Linear;
    Unit::set(controller, &min, "0.0", keyDependent);
    Unit::set(controller, &max, "1.0", keyDependent);
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
}

UnitParameter::~UnitParameter() {
    delete[] output;
}

void UnitParameter::apply(Instrument* instrument) {
    min->update(instrument);
    max->update(instrument);
    
    // Linearly interpolate between min and max
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = Interpolation::ease(min->output[x], max->output[x], value, curve);
}

bool UnitParameter::setValue(std::string parameter, std::string value) {
    if(parameter.compare("min") == 0)
        return Unit::set(controller, &min, value, keyDependent);
    
    if(parameter.compare("max") == 0)
        return Unit::set(controller, &max, value, keyDependent);
    
    if(parameter.compare("curve") == 0)
        return Interpolation::set(controller, &curve, value);
    
    if(parameter.compare("position") == 0) {
        if(!Util::isNumber(value)) return false;
        
        this->value = stod(value);
        if(this->value < 0.0) this->value = 0.0;
        if(this->value > 1.0) this->value = 1.0;
        return true;
    }
    
    return false;
}

void UnitParameter::setValue(double value) {
    this->value = value;
}
