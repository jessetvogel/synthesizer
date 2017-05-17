#include "ADSR.hpp"

#include <iostream>

ADSR::ADSR() {
    // Set default values
    attack = 0.0;
    decay = 0.0;
    sustain = 1.0;
    release = 0.0;
}

float ADSR::value(bool pressing, double duration, double release) {
    // TODO: implement other interpolation kind of things
    
    // Attack stage
    float amplitude;
    if(duration < attack)
        amplitude = duration / attack;
    
    // Decay stage
    else if(duration < attack + decay)
        amplitude = 1.0f - (1.0f - sustain) * (duration - attack) / decay;
    
    // Sustain stage
    else amplitude = sustain;

    if(pressing) {
        finished = false;
        return amplitude;
    }
    
    // Release stage
    else {
        amplitude -= sustain * release / (this->release);
        if(amplitude <= 0.0f) {
            finished = true;
            amplitude = 0.0;
        }
        return amplitude;
    }
}