#include <cmath>

#include "settings.hpp"

Settings::Settings() {
    // In case some settings were not explicitly set, use the defaults
    // Set default values
    for(int i = 0;i < AMOUNT_OF_KEYS; i ++) {
        frequencies[i] = 440.0 * pow(2.0, (double) (i - NOTE_A4) / 12.0);
    }
    
    sampleRate = DEFAULT_SAMPLE_RATE;
    bufferSize = DEFAULT_BUFFER_SIZE;
    
    pitchWheelRange = DEFAULT_PITCH_WHEEL_RANGE;
    sustainPedalPolarity = DEFAULT_SUSTAIN_PEDAL_POLARITY;
}

