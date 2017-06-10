#include "unitADSR.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "midistate.hpp"

UnitADSR::UnitADSR(Controller* controller, bool keyDependent) {
    // Store pointer to controller
    this->controller = controller;
    
    // An ADSR may or may not be key dependent
    this->keyDependent = keyDependent;
    
    // Set default values
    Unit::set(controller, &startLevel, "0.0", keyDependent);
    Unit::set(controller, &attackLevel, "1.0", keyDependent);
    Unit::set(controller, &sustainLevel, "1.0", keyDependent);
    Unit::set(controller, &releaseLevel, "0.0", keyDependent);
    
    Unit::set(controller, &attackTime, "0.0", keyDependent);
    Unit::set(controller, &decayTime, "0.0", keyDependent);
    Unit::set(controller, &releaseTime, "0.0", keyDependent);
    
    if(keyDependent) {
        Unit::set(controller, &duration, "key_duration", keyDependent);
        Unit::set(controller, &release, "key_release", keyDependent);
    }
    else {
        Unit::set(controller, &duration, "lead_key_duration", keyDependent);
        Unit::set(controller, &release, "lead_key_release", keyDependent);
    }
    
    attackType = Interpolation::Linear;
    decayType = Interpolation::Linear;
    releaseType = Interpolation::Linear;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
}

UnitADSR::~UnitADSR() {
    delete[] output;
}

void UnitADSR::apply(Instrument* instrument) {
    startLevel->update(instrument);
    attackLevel->update(instrument);
    sustainLevel->update(instrument);
    releaseLevel->update(instrument);
    attackTime->update(instrument);
    decayTime->update(instrument);
    releaseTime->update(instrument);
    duration->update(instrument);
    release->update(instrument);
    
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x) {
        
        double attack = attackTime->output[x];
        double decay = decayTime->output[x];
        double releaseT = releaseTime->output[x];
        
        double d = duration->output[x];
        double r = release->output[x];
        
        float amplitude;
        
        // Attack stage
        if(d < attack)
            amplitude = Interpolation::ease(startLevel->output[x], attackLevel->output[x], d / attack, attackType);
        
        // Decay stage
        else if(d < attack + decay)
            amplitude = Interpolation::ease(attackLevel->output[x], sustainLevel->output[x], (d - attack) / decay, decayType);
        
        // Sustain stage
        else amplitude = sustainLevel->output[x];
        
        // Release stage
        if(r > 0) { // TODO: better way to see this?
            if(r >= releaseT)
                amplitude = releaseLevel->output[x];
            else
                amplitude = Interpolation::ease(amplitude, releaseLevel->output[x], r / releaseT, releaseType);
        }
        
        output[x] = amplitude;
    }
}

bool UnitADSR::setValue(std::string parameter, std::string value) {
    if(parameter.compare("start_level") == 0)
        return Unit::set(controller, &startLevel, value, keyDependent);
    
    if(parameter.compare("attack_level") == 0)
        return Unit::set(controller, &attackLevel, value, keyDependent);

    if(parameter.compare("sustain_level") == 0)
        return Unit::set(controller, &sustainLevel, value, keyDependent);
    
    if(parameter.compare("release_level") == 0)
        return Unit::set(controller, &releaseLevel, value, keyDependent);
    
    if(parameter.compare("attack_time") == 0)
        return Unit::set(controller, &attackTime, value, keyDependent);
    
    if(parameter.compare("decay_time") == 0)
        return Unit::set(controller, &decayTime, value, keyDependent);
    
    if(parameter.compare("release_time") == 0)
        return Unit::set(controller, &releaseTime, value, keyDependent);
    
    if(parameter.compare("attack_type") == 0)
        return Interpolation::set(controller, &attackType, value);
    
    if(parameter.compare("decay_type") == 0)
        return Interpolation::set(controller, &decayType, value);
    
    if(parameter.compare("release_type") == 0)
        return Interpolation::set(controller, &releaseType, value);
    
    return false;
};
