#include "instrument.hpp"
#include <cstring>
#include <cmath>

Instrument::Instrument(Controller* controller) {
    // Store pointer to controller object
    this->controller = controller;
    
    // Initialize arrays
    buffer = new float[controller->getFramesPerBuffer()];
    phase = new double[AMOUNT_OF_NOTES];
    memset(phase, 0, sizeof(double) * AMOUNT_OF_NOTES);
}

Instrument::~Instrument() {
    delete[] buffer;
}

void Instrument::update(MidiState* midiState) {
    // First clear the buffer
    memset(buffer, 0, sizeof(float) * controller->getFramesPerBuffer());
    
    for(int i = 0;i < AMOUNT_OF_NOTES;i ++) {
        if(midiState->velocity[i] > 0)
            updateNote(midiState, i);
    }
}

void Instrument::updateNote(MidiState* midiState, int i) {
    unsigned long framesPerBuffer = controller->getFramesPerBuffer();
    Settings* settings = controller->getSettings();
    double sampleRate = controller->getSampleRate();
    
    // Determine information for this part
    double frequency = settings->frequencies[i];
    if(midiState->pitchWheel != 0.0) {
        frequency *= pow(2.0, midiState->pitchWheel * settings->pitchWheelRange / 12.0);
    }
    double amplitude = sqrt((midiState->velocity[i] + 1) / 128.0) * midiState->mainVolume;
    
    double phasePerFrame = 2.0 * M_PI * frequency / sampleRate;
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        double value = sample->getValue(phase[i] + phasePerFrame * x);
        if(midiState->modulationWheel > 0.0) {
            double distortedValue = value * value * value;
            distortedValue = distortedValue * distortedValue * distortedValue;
            value = value * (1.0 - midiState->modulationWheel) + distortedValue * midiState->modulationWheel;
        }

        buffer[x] += amplitude * value;
    }
    
    phase[i] += phasePerFrame * framesPerBuffer;
}

void Instrument::addBuffer(float* buffer) {
    unsigned long framesPerBuffer = controller->getFramesPerBuffer();
    for(int i = 0;i < framesPerBuffer; ++i)
        buffer[i] += this->buffer[i];
}

void Instrument::setSample(Sample* sample) {
    this->sample = sample;
}
