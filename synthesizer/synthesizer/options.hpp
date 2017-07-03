#ifndef options_hpp
#define options_hpp

#define NOTE_A4 (69)

#define DEFAULT_PITCH_WHEEL_RANGE (2.0)
#define DEFAULT_SUSTAIN_PEDAL_POLARITY (false)

#include <string>

#include "settings.hpp"

class Controller;

class Options {
    
    Controller* controller;
    
public:
    
    Options(Controller*);
    
    double frequencies[AMOUNT_OF_KEYS];
    
    double pitchWheelRange;
    bool sustainPedalPolarity;
    
    bool setSustainPedalPolarity(std::string);
    bool setPitchWheelRange(int);
    
    
//    enum Tuning { EQUAL_TEMPERAMENT, PYTHAGOREAN };
//    void setTuning(Tuning); TODO
    
};


#endif
