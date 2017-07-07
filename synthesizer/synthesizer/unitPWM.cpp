#include <cmath>

#include "unitPWM.hpp"
#include "controller.hpp"
#include "settings.hpp"
#include "units.hpp"
#include "parameter.hpp"
#include "settings.hpp"
#include "arguments.hpp"

UnitPWM::UnitPWM(Controller* controller, Arguments arguments) : Unit(controller) {
    // Set type
    type = "PWM";
    
    // Set arguments
    keyDependent = arguments.getBool("key", false);
    
    // Set parameters
    parameters.push_back(frequency = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "frequency", "0.0"));
    parameters.push_back(duty = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "duty", "0.0"));
    parameters.push_back(low = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "low", "0.0"));
    parameters.push_back(high = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "high", "1.0"));
    
    // Create arrays
    if(keyDependent) {
        phase = new double[SETTINGS_POLYPHONY];
        memset(phase, 0, sizeof(double) * SETTINGS_POLYPHONY);
    }
    else {
        phase = new double[1];
        phase[0] = 0.0;
    }
}

UnitPWM::~UnitPWM() {
    delete[] phase;
}

void UnitPWM::apply() {
    Unit* frequency = (Unit*) (this->frequency->pointer);
    Unit* duty = (Unit*) (this->duty->pointer);
    Unit* low = (Unit*) (this->low->pointer);
    Unit* high = (Unit*) (this->high->pointer);
   
    double t = 1.0 / sampleRate;
    int i = keyDependent ? controller->getUnits()->currentKey->id : 0;
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        output[x] = phase[i] < duty->output[x] ? high->output[x] : low->output[x];
        phase[i] += t * frequency->output[x];
        if(phase[i] >= 1.0) phase[i] -= floor(phase[i]);
    }
}
