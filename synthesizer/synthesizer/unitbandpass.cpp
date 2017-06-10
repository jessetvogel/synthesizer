#include "unitbandpass.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "settings.hpp"

UnitBandPass::UnitBandPass(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // May or may not be key dependent
    this->keyDependent = false;
    
    // Set default values
    Unit::set(controller, &input, "0.0", keyDependent);
    Unit::set(controller, &centerFrequency, "1000.0", keyDependent);
    Unit::set(controller, &qFactor, "1.0", keyDependent); // TODO: come up with some standard values
    input_1 = 0.0;
    input_2 = 0.0;
    output_1 = 0.0;
    output_2 = 0.0;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
}

void UnitBandPass::apply(Instrument* instrument) {
    input->update(instrument);
    centerFrequency->update(instrument);
    qFactor->update(instrument);
    
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

bool UnitBandPass::setValue(std::string parameter, std::string value) {
    if(parameter.compare("input") == 0)
        return Unit::set(controller, &input, value, keyDependent);
    
    if(parameter.compare("center") == 0)
        return Unit::set(controller, &centerFrequency, value, keyDependent);
    
    if(parameter.compare("q_factor") == 0)
        return Unit::set(controller, &qFactor, value, keyDependent);
    
    return false;
}
