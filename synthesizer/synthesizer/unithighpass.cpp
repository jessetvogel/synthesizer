#include <cmath>

#include "unithighpass.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "parameter.hpp"
#include "settings.hpp"

UnitHighpass::UnitHighpass(Controller* controller) : Unit(controller) {
    // Set type
    type = "highpass";
    
    // May or may not be key dependent
    this->keyDependent = false;
    
    // Set default values
    parameters.push_back(input = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "input", "0.0"));
    parameters.push_back(cutOffFrequency = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "cutoff", "1000.0"));
    input_1 = 0.0;
    output_1 = 0.0;
}

void UnitHighpass::apply(Instrument* instrument) {
    Unit* input = (Unit*) (this->input->pointer);
    Unit* cutOffFrequency = (Unit*) (this->cutOffFrequency->pointer);
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        // Clearly, this makes total sense! (for those who do not understand: https://en.wikipedia.org/wiki/High-pass_filter, and use that RC = 1/(2*pi*f_c) and dt = 1/sampleRate )
        double alpha = 1.0 / (1.0 + (2.0 * M_PI * cutOffFrequency->output[x]) / sampleRate);
        output[x] = alpha * ((x == 0 ? output_1 : output[x - 1]) + input->output[x] - (x == 0 ? input_1 : input->output[x - 1]));
    }
    output_1 = output[framesPerBuffer - 1];
    input_1 = input->output[framesPerBuffer - 1];
}
