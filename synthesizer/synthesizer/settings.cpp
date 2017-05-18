#include <cmath>

#include "settings.hpp"

Settings::Settings() {
    // Set default values
    for(int i = 0;i < AMOUNT_OF_KEYS; i ++) {
        frequencies[i] = 440.0 * pow(2.0, (double) (i - NOTE_A4) / 12.0);
    }
    
    pitchWheelRange = 2;
    invertedSustainPedal = true;
}
