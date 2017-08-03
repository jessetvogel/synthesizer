#ifndef keystate_hpp
#define keystate_hpp

#include <portmidi.h>
#include "settings.hpp"

#define MIDI_NOTE_OFF               (0x8)
#define MIDI_NOTE_ON                (0x9)
#define MIDI_POLYPHONIC_AFTERTOUCH  (0xA)
#define MIDI_CONTROL_CHANGE         (0xB)
#define MIDI_PROGRAM_CHANGE         (0xC)
#define MIDI_CHANNEL_AFTERTOUCH     (0xD)
#define MIDI_PITCH_WHEEL            (0xE)

#define MIDI_MODULATION_WHEEL       (0x01)
#define MIDI_MAIN_VOLUME            (0x07)
#define MIDI_BALANCE                (0x08)
#define MIDI_PAN                    (0x0A)
#define MIDI_SUSTAIN_PEDAL          (0x40)

class Controller;
class Settings;

struct KeyEvent {
  
    enum Type { Pressed, Released };
    
    Type type;
    unsigned char key;
    
};

class KeyState {
    
    Controller* controller;
    
    double previousVelocity[AMOUNT_OF_KEYS];

public:
    
    KeyState(Controller*);
    
    double pitchWheel;
    double modulationWheel;
    double sustainPedal;
    
    double velocity[AMOUNT_OF_KEYS];
    double frequency[AMOUNT_OF_KEYS];
    double duration[AMOUNT_OF_KEYS];
    
    void addMIDIEvent(unsigned char, unsigned char, unsigned char);
    void update();
    
};

#endif
