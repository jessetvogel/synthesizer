#include "unitparameter.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "parameter.hpp"
#include "curve.hpp"
#include "util.hpp"

UnitParameter::UnitParameter(Controller* controller, int MidiCCNumber) : Unit(controller) {
    // Set type and other variables
    type = "parameter";
    this->MidiCCNumber = MidiCCNumber;
    
    // Not key dependent
    keyDependent = false;
    
    // Hidden
    hidden = true;
    
    // Set default values
    value = 0.5; // TODO
    parameters.push_back(min = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "min", "0.0"));
    parameters.push_back(max = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "max", "1.0"));
    parameters.push_back(curve = new Parameter(controller, Parameter::CURVE, "curve", "linear"));
}

void UnitParameter::apply(Instrument* instrument) {
    Unit* min = (Unit*) (this->min->pointer);
    Unit* max = (Unit*) (this->max->pointer);
    Curve* curve = (Curve*) (this->curve->pointer);
    
    // Linearly interpolate between min and max
    for(int x = 0;x < framesPerBuffer; ++x)
        output[x] = Curve::ease(min->output[x], max->output[x], value, curve);
}

void UnitParameter::setValue(double value) {
    this->value = value;
}
