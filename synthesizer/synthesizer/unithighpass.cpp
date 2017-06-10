#include "unithighpass.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "settings.hpp"

UnitHighpass::UnitHighpass(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // May or may not be key dependent
    this->keyDependent = keyDependent;
    
    // Set default values
    Unit::set(controller, &input, "0.0", keyDependent);
    Unit::set(controller, &cutOffFrequency, "1000.0", keyDependent);
    input_1 = 0.0;
    output_1 = 0.0;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
}

void UnitHighpass::apply(Instrument* instrument) {
    input->update(instrument);
    cutOffFrequency->update(instrument);
    
    unsigned long framesPerBuffer = controller->getFramesPerBuffer();
    for(int x = 0;x < framesPerBuffer; ++x) {
        // Clearly, this makes total sense! (for those who do not understand: https://en.wikipedia.org/wiki/High-pass_filter, and use that RC = 1/(2*pi*f_c) and dt = 1/sampleRate )
        double alpha = 1.0 / (1.0 + (2.0 * M_PI * cutOffFrequency->output[x]) / controller->getSampleRate());
        output[x] = alpha * ((x == 0 ? output_1 : output[x - 1]) + input->output[x] - (x == 0 ? input_1 : input->output[x - 1]));
    }
    output_1 = output[framesPerBuffer - 1];
    input_1 = input->output[framesPerBuffer - 1];
}

bool UnitHighpass::setValue(std::string parameter, std::string value) {
    if(parameter.compare("input") == 0)
        return Unit::set(controller, &input, value, keyDependent);
    
    if(parameter.compare("cutoff") == 0)
        return Unit::set(controller, &cutOffFrequency, value, keyDependent);
    
    return false;
}
