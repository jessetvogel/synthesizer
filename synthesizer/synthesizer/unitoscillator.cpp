#include <cmath>

#include "unitoscillator.hpp"
#include "controller.hpp"
#include "settings.hpp"
#include "units.hpp"
#include "parameter.hpp"
#include "sample.hpp"
#include "settings.hpp"
#include "arguments.hpp"

UnitOscillator::UnitOscillator(Controller* controller, Arguments arguments) : Unit(controller) {
    // Set type
    type = "oscillator";
    
    // Set arguments
    keyDependent = arguments.getBool("key", false);
    
    // Set parameters
    parameters.push_back(sample = new Parameter(controller, Parameter::SAMPLE, "sample", "sine"));
    parameters.push_back(frequency = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "frequency", "0.0"));
    parameters.push_back(amplitude = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "amplitude", "1.0"));
    parameters.push_back(mean = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "mean", "0.0"));
    
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

UnitOscillator::~UnitOscillator() {
    delete[] phase;
}

void UnitOscillator::apply() {
    Sample* sample = (Sample*) (this->sample->pointer);
    Unit* frequency = (Unit*) (this->frequency->pointer);
    Unit* amplitude = (Unit*) (this->amplitude->pointer);
    Unit* mean = (Unit*) (this->mean->pointer);
    
    double t = 1.0 / sampleRate;
    int i = keyDependent ? controller->getUnits()->currentKey->id : 0;
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        output[x] = mean->output[x] + amplitude->output[x] * sample->getValue(phase[i]);
        phase[i] += t * frequency->output[x] * 2.0 * M_PI;
    }
}
