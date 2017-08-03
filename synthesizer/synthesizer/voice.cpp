#include "voice.hpp"

#include <iostream>

bool Voice::idUsed[SETTINGS_MAX_VOICES] = {};

Voice::Voice() {
    // Look for an unused id
    for(int i = 0;i < SETTINGS_MAX_VOICES; ++i) {
        if(!idUsed[i]) {
            id = i;
            idUsed[i] = true;
            return;
        }
    }
    
    // Set default values
    key = 0;
    stage = Off;
    velocity = 0.0;
    frequency = 0.0;
    duration = 0.0;
    release = 0.0;
}

Voice::~Voice() {
    idUsed[id] = false;
}
