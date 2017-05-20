#include "unitoscillator.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "sample.hpp"
#include "settings.hpp"

UnitOscillator::UnitOscillator(Controller* controller, bool keyDependent) {
    // Store pointer to controller
    this->controller = controller;
    
    // May or may not be key dependent
    this->keyDependent = keyDependent;
    
    // Set default values
    Unit::set(controller, &frequency, "0.0", keyDependent);
    Unit::set(controller, &amplitude, "1.0", keyDependent);
    Unit::set(controller, &mean, "0.0", keyDependent);
    sample = Sample::sine;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    if(keyDependent) {
        phase = new double[AMOUNT_OF_KEYS];
        memset(phase, 0, sizeof(double) * AMOUNT_OF_KEYS);
    }
    else {
        phase = new double[1];
        phase[0] = 0.0;
    }
}

UnitOscillator::~UnitOscillator() {
    delete[] phase;
    delete[] output;
}

void UnitOscillator::apply(Instrument* instrument) {
    frequency->update(instrument);
    amplitude->update(instrument);
    mean->update(instrument);
    
    double t = 1.0 / controller->getSampleRate();
    int i = keyDependent ? instrument->currentKey : 0;
    
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x) {
        output[x] = mean->output[x] + amplitude->output[x] * sample->getValue(phase[i]);
        phase[i] += t * frequency->output[x] * 2.0 * M_PI;
    }
}

bool UnitOscillator::setValue(std::string parameter, std::string value) {
    if(parameter.compare("sample") == 0)
        return Sample::set(controller, &sample, value);
    
    if(parameter.compare("frequency") == 0)
        return Unit::set(controller, &frequency, value, keyDependent);
    
    if(parameter.compare("amplitude") == 0)
        return Unit::set(controller, &amplitude, value, keyDependent);
    
    if(parameter.compare("mean") == 0)
        return Unit::set(controller, &mean, value, keyDependent);
    
    return false;
}
