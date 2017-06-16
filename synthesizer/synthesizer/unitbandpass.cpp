#include "unitbandpass.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "parameter.hpp"
#include "settings.hpp"

UnitBandPass::UnitBandPass(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    type = "bandpass";
    
    // May or may not be key dependent
    this->keyDependent = false;
    
    // Set default values
    parameters.push_back(input = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "input", "0.0"));
    parameters.push_back(centerFrequency = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "center", "1000.0"));
    parameters.push_back(qFactor = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "q_factor", "1.0")); // TODO: come up with some standard values
    input_1 = 0.0;
    input_2 = 0.0;
    output_1 = 0.0;
    output_2 = 0.0;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
}

void UnitBandPass::apply(Instrument* instrument) {
    Unit* input = (Unit*) (this->input->pointer);
    Unit* centerFrequency = (Unit*) (this->centerFrequency->pointer);
    Unit* qFactor = (Unit*) (this->qFactor->pointer);
    
    unsigned long framesPerBuffer = controller->getFramesPerBuffer();
    double i0, i2, o1, o2, alpha, beta;
    double s = controller->getSampleRate();
    for(int x = 0;x < framesPerBuffer; ++x) {
        i0 = input->output[x];
        i2 = (x == 0 ? input_2 : (x == 1 ? input_1 : input->output[x - 2]));
        o1 = (x == 0 ? output_1 : output[x - 1]);
        o2 = (x == 0 ? output_2 : (x == 1 ? output_1 : output[x - 2]));
        
        alpha = (M_PI * centerFrequency->output[x] / s / qFactor->output[x]); // TODO: according to audio effects.pdf this should be tan(...), but that gives rise to some problems?
        beta = 2.0 * cos(2.0 * M_PI * centerFrequency->output[x] / s);
        
        // Should make sense.
        output[x] = (alpha * (i0 - i2) + beta * o1 + (alpha - 1.0) * o2) / (alpha + 1.0);
    }
    
    input_1 = input->output[framesPerBuffer - 1];
    input_2 = input->output[framesPerBuffer - 2];
    output_1 = output[framesPerBuffer - 1];
    output_2 = output[framesPerBuffer - 2];
}
