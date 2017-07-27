#include <iostream>

#include "midistate.hpp"
#include "controller.hpp"
#include "nodes.hpp"
#include "settings.hpp"
#include "nodeparameter.hpp"

MidiState::MidiState(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Set default values
    for(int i = 0;i < AMOUNT_OF_KEYS;i ++) {
        velocity[i] = 0;
        previousVelocity[i] = 0;
        keyStage[i] = Off;
        keyFrequency[i] = 0.0;
        keyVelocity[i] = 0.0;
        keyDuration[i] = 0.0;
    }
    
    pitchWheel = 0.0;
    modulationWheel = 0.0;
    mainVolume = 1.0;
    sustainPedal = 0.0;
    
    leadKey.stage = KeyEvent::Off;
    leadKey.key = 0.0;
}

void MidiState::addEvent(unsigned char status, unsigned char data1, unsigned char data2) {
    
    unsigned char type = status >> 4;
//    unsigned char channel = status & 0xF; // Don't need channels for now TODO: future?
    
    switch(type) {
        case MIDI_NOTE_OFF:
            velocity[data1] = 0;
            return;
            
        case MIDI_NOTE_ON:
            velocity[data1] = data2;
            return;
        
        case MIDI_CONTROL_CHANGE:
            
            switch(data1) {
                case MIDI_MODULATION_WHEEL:
                    modulationWheel = (double) data2 / 127.0;
                    return;
                    
                case MIDI_MAIN_VOLUME:
                    mainVolume = (double) data2 / 127.0;
                    return;
                
                case MIDI_SUSTAIN_PEDAL:
                    sustainPedal = (double) data2 / 127.0;
                    if(controller->getSettings()->sustainPedalPolarity)
                        sustainPedal = 1.0 - sustainPedal;
                    return;
            }
            
            // If not one of the default, set belonging NodeParameter if it exists
            controller->getNodes()->updateNodeParameter(data1, (double) data2 / 127.0);
            return;
        
        case MIDI_PITCH_WHEEL:
            // TODO: make this somewhat cleaner
            pitchWheel = ((data2 << 7) | (data1)) / 8191.5 - 1.0;
            if(pitchWheel < 0.0001 && pitchWheel > -0.0001) pitchWheel = 0.0;
            return;
    }
 
    printf("UNKOWN MIDI:\t%#04x (%d)\t%#04x (%d)\t%#04x (%d)\n", status, status, data1, data1, data2, data2);
}

void MidiState::update() {
    Settings* settings = controller->getSettings();

    for(int i = 0;i < AMOUNT_OF_KEYS; ++i) {
        // Update frequencies
        keyFrequency[i] = (pitchWheel == 0.0) ? settings->frequencies[i] : settings->frequencies[i] * pow(2.0, pitchWheel * settings->pitchWheelRange / 12.0);
        
        // Update velocities
        if(velocity[i] > 0) keyVelocity[i] = (double) velocity[i] / 127.0;
        
        // Update durations
        if(keyVelocity[i] > 0) keyDuration[i] += settings->bufferSize / settings->sampleRate;
        
        // If we just now pressed key i, create a new key event
        if(previousVelocity[i] == 0 && velocity[i] > 0) {
            KeyEvent keyEvent;
            keyEvent.key = i;
            keyEvent.stage = KeyEvent::Press;
            keyEvent.duration = 0.0;
            keyEvent.release = 0.0;
            controller->getNodes()->addKeyEvent(&keyEvent);
            
            // Whenever pressed, reset the keyDuration
            keyDuration[i] = 0.0;
            
            // Use this key as the leading key
            leadKey.key = i;
            leadKey.stage = KeyEvent::Press;
            leadKey.duration = 0.0;
            leadKey.release = 0.0;
        }
        previousVelocity[i] = velocity[i];
    }
    
    // If lead key was released, but other keys are pressed, use the one that is pressed shortest ago
    if(velocity[leadKey.key] == 0 && sustainPedal < 0.5) {
//        leadKey.velocity = 0.0;
        double minDuration = 99999.0; // TODO: less magic..
        for(int i = 0;i < AMOUNT_OF_KEYS; ++i) {
            if(velocity[i] > 0 && keyDuration[i] < minDuration) {
                leadKey.key = i;
                leadKey.stage = KeyEvent::Press;
                leadKey.duration = keyDuration[i];
                leadKey.release = 0.0;
                minDuration = keyDuration[i];
            }
        }
    }
    
    // Update lead key
    updateKeyEvent(&leadKey);
}

void MidiState::updateKeyEvent(KeyEvent* keyEvent) {
    // Store local variables
    unsigned long framesPerBuffer = controller->getSettings()->bufferSize;
    double sampleRate = controller->getSettings()->sampleRate;
    
    // Update its duration and release time
    if(keyEvent->stage == KeyEvent::Press || keyEvent->stage == KeyEvent::Sustain)
        keyEvent->duration += framesPerBuffer / sampleRate;
    
    if(keyEvent->stage == KeyEvent::Released)
        keyEvent->release += framesPerBuffer / sampleRate;
    
    // Check if we should go to the sustain or release stage
    if((keyEvent->stage == KeyEvent::Press || keyEvent->stage == KeyEvent::Sustain) && velocity[keyEvent->key] == 0) {
        if(sustainPedal < 0.5)
            keyEvent->stage = KeyEvent::Released;
        else
            keyEvent->stage = KeyEvent::Sustain;
    }
    
    // Update its velocity and frequency
    if(keyEvent->stage == KeyEvent::Press || keyEvent->stage == KeyEvent::Sustain)
        keyEvent->velocity = keyVelocity[keyEvent->key];
    keyEvent->frequency = keyFrequency[keyEvent->key];
}
