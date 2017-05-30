#include "unitlowpass.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "settings.hpp"

UnitLowpass::UnitLowpass(Controller* controller, bool keyDependent) {
    // Store pointer to controller
    this->controller = controller;
    
    // May or may not be key dependent
    this->keyDependent = keyDependent;
    
    // Set default values
    Unit::set(controller, &input, "0.0", keyDependent);
    Unit::set(controller, &cutOffFrequency, "1000.0", keyDependent);
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
    
    if(keyDependent) {
        lastOutput = new float[MAX_AMOUNT_OF_IDS];
        memset(lastOutput, 0, sizeof(float) * MAX_AMOUNT_OF_IDS);
    }
    else {
        lastOutput = new float[1];
        lastOutput[0] = 0.0;
    }
}

void UnitLowpass::apply(Instrument* instrument) {
    input->update(instrument);
    cutOffFrequency->update(instrument);
    
    int i = keyDependent ? instrument->currentKey->id : 0;
    
    unsigned long framesPerBuffer = controller->getFramesPerBuffer();
    for(int x = 0;x < framesPerBuffer; ++x) {
        // Clearly, this makes total sense! (for those who do not understand: https://en.wikipedia.org/wiki/Low-pass_filter, and use that RC = 1/(2*pi*f_c) and dt = 1/sampleRate )
        double alpha = 1.0 / (1.0 + controller->getSampleRate() / (2.0 * M_PI * cutOffFrequency->output[x]));
        output[x] = alpha * input->output[x] + (1.0 - alpha) * (x == 0 ? lastOutput[i] : output[x - 1]);
    }
    lastOutput[i] = output[framesPerBuffer - 1];
}

bool UnitLowpass::setValue(std::string parameter, std::string value) {
    if(parameter.compare("input") == 0)
        return Unit::set(controller, &input, value, keyDependent);
    
    if(parameter.compare("cutoff") == 0)
        return Unit::set(controller, &cutOffFrequency, value, keyDependent);
    
    return false;
}
