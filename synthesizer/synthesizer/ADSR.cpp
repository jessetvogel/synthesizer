#include "ADSR.hpp"
#include "controller.hpp"
#include "instrument.hpp"

ADSR::ADSR(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Set default values
    attackTime = 0.0;
    decayTime = 0.0;
    sustainLevel = 1.0;
    releaseTime = 0.0;
    
    attackType = Interpolation::Linear;
    decayType = Interpolation::Linear;
    releaseType = Interpolation::Linear;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
}

ADSR::~ADSR() {
    delete[] output;
}

void ADSR::apply(Instrument* instrument) {
    double duration = instrument->currentDuration;
    double release = instrument->currentRelease;
    Instrument::Stage stage = instrument->currentStage;
    float amplitude;

    // Attack stage
    if(duration < attackTime)
        amplitude = Interpolation::ease(attackType, duration / attackTime);
    
    // Decay stage
    else if(duration < attackTime + decayTime)
        amplitude = 1.0f - (1.0f - sustainLevel) * Interpolation::ease(decayType, (duration - attackTime) / decayTime);
    
    // Sustain stage
    else amplitude = sustainLevel;

    // Release stage
    if(stage != Instrument::Press && stage != Instrument::Sustain) {
        if(release >= releaseTime)
            amplitude = 0.0;
        else
            amplitude = sustainLevel * (1.0f - Interpolation::ease(releaseType, release / releaseTime));
    }
    
    // TODO: make amplitude dependend on x instead of constant
    
    // ...
    input->update(instrument);
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = amplitude * input->output[x];
}

bool ADSR::setValue(std::string parameter, std::string value) {
    if(parameter.compare("attack_time") == 0) {
        attackTime = stod(value);
        return true;
    }
    
    if(parameter.compare("decay_time") == 0) {
        decayTime = stod(value);
        return true;
    }
    
    if(parameter.compare("sustain_level") == 0) {
        sustainLevel = stod(value);
        return true;
    }
    
    if(parameter.compare("release_time") == 0) {
        releaseTime = stod(value);
        return true;
    }
    
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
    
    if(parameter.compare("input") == 0) {
        KeyUnit* k = controller->getKeyUnit(value);
        if(k == NULL) return false;
        
        input = k;
        return true;
    }
    
    return false;
};
