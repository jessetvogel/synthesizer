#include "keyunitADSR.hpp"
#include "controller.hpp"
#include "instrument.hpp"

KeyUnitADSR::KeyUnitADSR(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Set default values
    KeyUnit::set(controller, &attackTime, "0.0");
    KeyUnit::set(controller, &decayTime, "0.0");
    KeyUnit::set(controller, &sustainLevel, "1.0");
    KeyUnit::set(controller, &releaseTime, "0.0");
    
    attackType = Interpolation::Linear;
    decayType = Interpolation::Linear;
    releaseType = Interpolation::Linear;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
}

KeyUnitADSR::~KeyUnitADSR() {
    delete[] output;
}

void KeyUnitADSR::apply(Instrument* instrument) {
    attackTime->update(instrument);
    decayTime->update(instrument);
    sustainLevel->update(instrument);
    releaseTime->update(instrument);
    
    double attack = attackTime->output[0];
    double decay = decayTime->output[0];
    double sustain = sustainLevel->output[0];
    double release = releaseTime->output[0];
    
    double d = instrument->currentDuration;
    double r = instrument->currentRelease;
    Instrument::Stage stage = instrument->currentStage;
    float amplitude;
    
    // Attack stage
    if(d < attack)
        amplitude = Interpolation::ease(attackType, d / attack);
    
    // Decay stage
    else if(d < attack + decay)
        amplitude = 1.0f - (1.0f - sustain) * Interpolation::ease(decayType, (d - attack) / decay);
    
    // Sustain stage
    else amplitude = sustain;

    // Release stage
    if(stage != Instrument::Press && stage != Instrument::Sustain) {
        if(r >= release)
            amplitude = 0.0;
        else
            amplitude = amplitude * (1.0f - Interpolation::ease(releaseType, r / release));
    }
    
    // TODO: make amplitude dependend on x instead of constant
    
    // ...
    input->update(instrument);
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = amplitude * input->output[x];
}

bool KeyUnitADSR::setValue(std::string parameter, std::string value) {
    if(parameter.compare("attack_time") == 0)
        return KeyUnit::set(controller, &attackTime, value);
    
    if(parameter.compare("decay_time") == 0)
        return KeyUnit::set(controller, &decayTime, value);
    
    if(parameter.compare("sustain_level") == 0)
        return KeyUnit::set(controller, &sustainLevel, value);
    
    if(parameter.compare("release_time") == 0)
        return KeyUnit::set(controller, &releaseTime, value);
    
    if(parameter.compare("attack_type") == 0) {
        attackType = Interpolation::fromString(value);
        return true;
    }
    
    if(parameter.compare("decay_type") == 0) {
        decayType = Interpolation::fromString(value);
        return true;
    }
    
    if(parameter.compare("release_type") == 0) {
        releaseType = Interpolation::fromString(value);
        return true;
    }
    
    if(parameter.compare("input") == 0)
        return KeyUnit::set(controller, &input, value);
    
    return false;
};
