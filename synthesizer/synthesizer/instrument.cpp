#include "instrument.hpp"
#include <cstring>
#include <cmath>

#include "log.hpp"

Instrument::Instrument(Controller* controller) {
    // Store pointer to controller object
    this->controller = controller;
    
    // Initialize arrays
    buffer = new float[controller->getFramesPerBuffer()];
    phase = new double[AMOUNT_OF_NOTES];
    memset(phase, 0, sizeof(double) * AMOUNT_OF_NOTES);
    stage = new Stage[AMOUNT_OF_NOTES];
    memset(stage, 0, sizeof(Stage) * AMOUNT_OF_NOTES);
    velocity = new unsigned char[AMOUNT_OF_NOTES];
    memset(velocity, 0, sizeof(unsigned char) * AMOUNT_OF_NOTES);
    duration = new double[AMOUNT_OF_NOTES];
    memset(duration, 0, sizeof(double) * AMOUNT_OF_NOTES);
    release = new double[AMOUNT_OF_NOTES];
    memset(release, 0, sizeof(double) * AMOUNT_OF_NOTES);
    
    // Set default values
    sample = Sample::sine;
    envelope = Envelope::standard;
}

Instrument::~Instrument() {
    delete[] buffer;
    delete[] phase;
    delete[] stage;
    delete[] velocity;
    delete[] duration;
    delete[] release;
}

void Instrument::update(MidiState* midiState) {
    // First clear the buffer
    memset(buffer, 0, sizeof(float) * controller->getFramesPerBuffer());
    
    for(int i = 0;i < AMOUNT_OF_NOTES;i ++) {
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
            if((midiState->sustainPedal < 0.5) != (controller->getSettings()->invertedSustainPedal))
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
        
        // Update all active notes. Note: stage will be set to Off if the envelope indicates so
        if(stage[i] != Off) updateNote(midiState, i);
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
    double amplitude = sqrt((velocity[i] + 1) / 128.0) * midiState->mainVolume;

    // Apply envelope
    double d = duration[i];
    double r = release[i];
    if(stage[i] == Press || stage[i] == Sustain)
        d += (double) i / sampleRate;

    if(stage[i] == Released)
        r += (double) i / sampleRate;
        
    amplitude *= envelope->value(stage[i] == Press || stage[i] == Sustain, d, r);
    if(envelope->finished) stage[i] = Off;
    
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

void Instrument::setEnvelope(Envelope* envelope) {
    this->envelope = envelope;
}
