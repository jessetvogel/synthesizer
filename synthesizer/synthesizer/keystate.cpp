#include "keystate.hpp"
#include "controller.hpp"
#include "nodes.hpp"
#include "settings.hpp"
#include "nodeparameter.hpp"

#include "log.hpp"

KeyState::KeyState(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Set default values
    for(int i = 0;i < AMOUNT_OF_KEYS;i ++) {
        velocity[i] = 0.0;
        previousVelocity[i] = 0.0;
        duration[i] = 0.0;
    }
    
    pitchWheel = 0.0;
    modulationWheel = 0.0;
    sustainPedal = 0.0;
}

void KeyState::addMIDIEvent(unsigned char status, unsigned char data1, unsigned char data2) {
    
    unsigned char type = status >> 4;
//    unsigned char channel = status & 0xF; // Don't need channels for now TODO: future? Hope so.
    
    switch(type) {
        case MIDI_NOTE_OFF:
            velocity[data1] = 0.0;
            return;
            
        case MIDI_NOTE_ON:
            velocity[data1] = (double) data2 / 127.0;
            return;
        
        case MIDI_CONTROL_CHANGE:
            
            switch(data1) {
                case MIDI_MODULATION_WHEEL:
                    modulationWheel = (double) data2 / 127.0;
                    return;
                
                case MIDI_SUSTAIN_PEDAL:
                    sustainPedal = (double) data2 / 127.0;
                    if(controller->getSettings()->sustainPedalPolarity)
                        sustainPedal = 1.0 - sustainPedal;
                    return;
            }
            
            if(data1 == controller->getSettings()->masterVolumeCC)
                controller->getSettings()->masterVolume = (double) data2 / 127.0;
            
            // If not one of the default, set belonging NodeParameter if it exists
            controller->getNodes()->updateNodeParameter(data1, (double) data2 / 127.0);
            return;
        
        case MIDI_PITCH_WHEEL:
            // TODO: make this somewhat cleaner
            pitchWheel = ((data2 << 7) | (data1)) / 8191.5 - 1.0;
            if(pitchWheel < 0.0001 && pitchWheel > -0.0001) pitchWheel = 0.0;
            return;
    }
 
    // TODO: remove this
    char buffer[256];
    sprintf(buffer, "UNKOWN MIDI:\t%#04x (%d)\t%#04x (%d)\t%#04x (%d)\n", status, status, data1, data1, data2, data2);
    Log::write(buffer);
}

void KeyState::update() {
    Settings* settings = controller->getSettings();
    
    for(int i = 0;i < AMOUNT_OF_KEYS; ++i) {
        // Update frequencies
        frequency[i] = (pitchWheel == 0.0) ? settings->frequencies[i] : settings->frequencies[i] * pow(2.0, pitchWheel * settings->pitchWheelRange / 12.0);
        
        // Update durations
        if(velocity[i] > 0.0)
            duration[i] += settings->bufferSize / settings->sampleRate;
        
        // In case key is pressed
        if(previousVelocity[i] == 0.0 && velocity[i] > 0.0) {
            // Reset duration
            duration[i] = 0.0;
            
            // Send a key event
            KeyEvent event;
            event.type = KeyEvent::Pressed;
            event.key = i;
            controller->getNodes()->addKeyEvent(&event);
        }
        
        // In case key is released
        if(previousVelocity[i] > 0.0 && velocity[i] == 0.0) {
            // Reset duration
            duration[i] = 0.0;
            
            // Send a key event
            KeyEvent event;
            event.type = KeyEvent::Released;
            event.key = i;
            controller->getNodes()->addKeyEvent(&event);
        }
        
        // Update previous velocities
        previousVelocity[i] = velocity[i];
    }
}
