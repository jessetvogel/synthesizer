#include "keyunitoscillator.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "sample.hpp"
#include "settings.hpp"

KeyUnitOscillator::KeyUnitOscillator(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Set default values
    KeyUnit::set(controller, &frequency, "0.0");
    KeyUnit::set(controller, &amplitude, "1.0");
    KeyUnit::set(controller, &mean, "0.0");
    sample = Sample::sine;
    
    // Create arrays
    phase = new double[AMOUNT_OF_KEYS];
    memset(phase, 0, sizeof(double) * AMOUNT_OF_KEYS);
    output = new float[controller->getFramesPerBuffer()];
}

KeyUnitOscillator::~KeyUnitOscillator() {
    delete[] phase;
    delete[] output;
}

void KeyUnitOscillator::apply(Instrument* instrument) {
    frequency->update(instrument);
    amplitude->update(instrument);
    mean->update(instrument);

    double t = 1.0 / controller->getSampleRate();
    
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x) {
        output[x] = mean->output[x] + amplitude->output[x] * sample->getValue(phase[instrument->currentKey]);
        phase[instrument->currentKey] += t * frequency->output[x] * 2.0 * M_PI;
    }
}

bool KeyUnitOscillator::setValue(std::string parameter, std::string value) {
    if(parameter.compare("sample") == 0) {
        Sample* s = Sample::fromString(value);
        if(s == NULL) return false;
        
        sample = s;
        return true;
    }
    
    if(parameter.compare("frequency") == 0)
        return KeyUnit::set(controller, &frequency, value);
    
    if(parameter.compare("amplitude") == 0)
        return KeyUnit::set(controller, &amplitude, value);
    
    if(parameter.compare("mean") == 0)
        return KeyUnit::set(controller, &mean, value);
    
    return false;
}
