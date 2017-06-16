#include "unitlowpass.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "parameter.hpp"

UnitLowpass::UnitLowpass(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    type = "lowpass";
    
    // Not key dependent
    this->keyDependent = false;
    
    // Set default values
    parameters.push_back(input = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "input", "0.0"));
    parameters.push_back(cutOffFrequency = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "cutoff", "1000.0"));
    output_1 = 0.0;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
}

void UnitLowpass::apply(Instrument* instrument) {
    Unit* input = (Unit*) (this->input->pointer);
    Unit* cutOffFrequency = (Unit*) (this->cutOffFrequency->pointer);
    
    unsigned long framesPerBuffer = controller->getFramesPerBuffer();
    for(int x = 0;x < framesPerBuffer; ++x) {
        // Clearly, this makes total sense! (for those who do not understand: https://en.wikipedia.org/wiki/Low-pass_filter, and use that RC = 1/(2*pi*f_c) and dt = 1/sampleRate )
        double alpha = 1.0 / (1.0 + controller->getSampleRate() / (2.0 * M_PI * cutOffFrequency->output[x]));
        output[x] = alpha * input->output[x] + (1.0 - alpha) * (x == 0 ? output_1 : output[x - 1]);
    }
    
    output_1 = output[framesPerBuffer - 1];
}
