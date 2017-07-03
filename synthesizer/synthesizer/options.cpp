#include <cmath>

#include "options.hpp"

Options::Options(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Set default values
    for(int i = 0;i < AMOUNT_OF_KEYS; i ++) {
        frequencies[i] = 440.0 * pow(2.0, (double) (i - NOTE_A4) / 12.0);
    }
    
    pitchWheelRange = DEFAULT_PITCH_WHEEL_RANGE;
    sustainPedalPolarity = DEFAULT_SUSTAIN_PEDAL_POLARITY;
}

bool Options::setSustainPedalPolarity(std::string polarity) {
    if(polarity.compare("inverted") == 0) {
        sustainPedalPolarity = true;
        return true;
    }
    
    if(polarity.compare("normal") == 0) {
        sustainPedalPolarity = false;
        return true;
    }
    
    return false;
}

bool Options::setPitchWheelRange(int range) {
    pitchWheelRange = range;
    return true;
}
