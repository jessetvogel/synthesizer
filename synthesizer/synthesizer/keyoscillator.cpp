#include "keyoscillator.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "sample.hpp"
#include "settings.hpp"

KeyOscillator::KeyOscillator(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Set default values
    // ...
    
    // Create arrays
    phase = new double[AMOUNT_OF_KEYS];
    memset(phase, 0, sizeof(double) * AMOUNT_OF_KEYS);
    output = new float[controller->getFramesPerBuffer()];
}

KeyOscillator::~KeyOscillator() {
    delete[] phase;
    delete[] output;
}

void KeyOscillator::apply(Instrument* instrument) {
    frequency->update(instrument);
    double f = frequency->output[0];
    double t = 1.0 / controller->getSampleRate();
    double p = phase[instrument->currentKey];
    
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = sample->getValue(p + t * x * f * 2.0 * M_PI);
    
    phase[instrument->currentKey] += t * controller->getFramesPerBuffer() * f * 2.0 * M_PI;
}

bool KeyOscillator::setValue(std::string parameter, std::string value) {
    if(parameter.compare("sample") == 0) {
        Sample* s = Sample::fromString(value);
        if(s == NULL) return false;
        
        sample = s;
        return true;
    }
    
    if(parameter.compare("frequency") == 0) {
        KeyUnit* k = controller->getKeyUnit(value);
        if(k == NULL) return false;
        
        frequency = k;
        return true;
    }
    
    return false;
}
