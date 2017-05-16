#include "midistate.hpp"

#include <iostream>
#include "log.hpp"

MidiState::MidiState() {
    // Set default values
    for(int i = 0;i < AMOUNT_OF_NOTES;i ++)
        velocity[i] = 0;
    
    pitchWheel = 0.0;
    modulationWheel = 0.0;
    mainVolume = 1.0;
    sustainPedal = 0.0;
}

void MidiState::update(unsigned char status, unsigned char data1, unsigned char data2) {
    unsigned char type = status >> 4;
//    unsigned char channel = status & 0xF; // Don't need channels for now
    
    switch(type) {
        case MIDI_NOTE_OFF:
            velocity[data1] = 0;
            break;
            
        case MIDI_NOTE_ON:
            velocity[data1] = data2;
            break;    
            
        case MIDI_POLYPHONIC_AFTERTOUCH:
            // TODO, but I don't know what it is
            break;
        
        case MIDI_CONTROL_CHANGE:
            switch(data1) {
                case MIDI_MODULATION_WHEEL:
                    modulationWheel = (double) data2 / 127.0;
                    break;
                    
                case MIDI_MAIN_VOLUME:
                    mainVolume = (double) data2 / 127.0;
                    break;
                    
                case MIDI_PAN:
                    // TODO, but I don't know what it is
                    break;
                    
                case MIDI_BALANCE:
                    // TODO, but I don't know what it is
                    break;
                
                case MIDI_SUSTAIN_PEDAL:
                    sustainPedal = data2 / 127.0;
                    break;
            }
            break;
            
        case MIDI_PROGRAM_CHANGE:
            // TODO, but I don't know what it is
            break;
            
        case MIDI_CHANNEL_AFTERTOUCH:
            // TODO, but I don't know what it is
            break;
            
        case MIDI_PITCH_WHEEL:
            // TODO: make this somewhat cleaner
            pitchWheel = ((data2 << 7) | (data1)) / 8191.5 - 1.0;
            if(pitchWheel < 0.0001 && pitchWheel > -0.0001) pitchWheel = 0.0;
            break;
    }
}