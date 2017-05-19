#include <cstring>
#include <cmath>

#include "instrument.hpp"
#include "controller.hpp"
#include "input.hpp"
#include "midistate.hpp"
#include "unit.hpp"
#include "keyunit.hpp"

#include "log.hpp"

Instrument::Instrument(Controller* controller) {
    // Store pointer to controller object
    this->controller = controller;
    
    // Initialize arrays
    buffer = new float[controller->getFramesPerBuffer()];
    stage = new Stage[AMOUNT_OF_KEYS];
    memset(stage, 0, sizeof(Stage) * AMOUNT_OF_KEYS);
    velocity = new unsigned char[AMOUNT_OF_KEYS];
    memset(velocity, 0, sizeof(unsigned char) * AMOUNT_OF_KEYS);
    duration = new double[AMOUNT_OF_KEYS];
    memset(duration, 0, sizeof(double) * AMOUNT_OF_KEYS);
    release = new double[AMOUNT_OF_KEYS];
    memset(release, 0, sizeof(double) * AMOUNT_OF_KEYS);
    
    // Default values
    releaseTime = 0.0;
    keyOutput = NULL;
}

Instrument::~Instrument() {
    delete[] buffer;
    delete[] stage;
    delete[] velocity;
    delete[] duration;
    delete[] release;
}

void Instrument::update(MidiState* midiState) {
    // First clear the buffer
//    memset(buffer, 0, sizeof(float) * controller->getFramesPerBuffer()); TODO
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x) buffer[x] = 0.0f;
    
    for(int i = 0;i < AMOUNT_OF_KEYS;i ++) {
        // Update duration and release time
        if(stage[i] == Press || stage[i] == Sustain)
            duration[i] += controller->getFramesPerBuffer() / controller->getSampleRate();

        if(stage[i] == Released)
            release[i] += controller->getFramesPerBuffer() / controller->getSampleRate();
        
        // Check if notes should be 'activated'
        if(stage[i] == Off && midiState->velocity[i] > 0) {
            stage[i] = Press;
            velocity[i] = midiState->velocity[i];
            duration[i] = 0.0;
            release[i] = 0.0;
        }

        // Check if we should go to the sustain or release stage
        if((stage[i] == Press || stage[i] == Sustain) && midiState->velocity[i] == 0) {
            if(midiState->sustainPedal < 0.5)
                stage[i] = Released;
            else
                stage[i] = Sustain;
        }
        
        // Check if pressed again while being in sustain stage
        if((stage[i] == Sustain || stage[i] == Released) && midiState->velocity[i] > 0) {
            stage[i] = Press;
            velocity[i] = midiState->velocity[i];
            duration[i] = 0.0;
            release[i] = 0.0;
        }
        
        // Check if done
        if(stage[i] == Released && release[i] > releaseTime)
            stage[i] = Off;
        
        // Update all active notes. Note: stage will be set to Off if the envelope indicates so
        if(stage[i] != Off) updateNote(midiState, i);
    }
}

void Instrument::updateNote(MidiState* midiState, int i) {
    Settings* settings = controller->getSettings();
    unsigned long framesPerBuffer = controller->getFramesPerBuffer();
    
    // Determine information for this part
    double frequency = settings->frequencies[i];
    if(midiState->pitchWheel != 0.0)
        frequency *= pow(2.0, midiState->pitchWheel * settings->pitchWheelRange / 12.0);

    double amplitude = sqrt((velocity[i] + 1) / 128.0) * midiState->mainVolume;

    if(keyOutput != NULL) {
        // Store information
        currentStage = stage[i];
        currentVelocity = velocity[i];
        currentDuration = duration[i];
        currentRelease = release[i];
        currentFrequency = frequency;
        currentKey = i;
        
        controller->resetKeyUnits();
        keyOutput->update(this);
    
        // Add output to the buffer
        for(int x = 0;x < framesPerBuffer; ++x)
            buffer[x] += amplitude * keyOutput->output[x];
    }
}

void Instrument::setReleaseTime(double releaseTime) {
    this->releaseTime = releaseTime;
}

void Instrument::setKeyOutput(KeyUnit* keyUnit) {
    this->keyOutput = keyUnit;
}

void Instrument::setOutput(Unit* unit) {
    this->output = unit;
}

void Instrument::addBuffer(float* buffer) {
    unsigned long framesPerBuffer = controller->getFramesPerBuffer();
    for(int i = 0;i < framesPerBuffer; ++i)
        buffer[i] += this->buffer[i];
}
