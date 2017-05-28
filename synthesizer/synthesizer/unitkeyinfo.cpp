#include "unitkeyinfo.hpp"
#include "controller.hpp"
#include "instrument.hpp"

UnitKeyInfo::UnitKeyInfo(Controller* controller, Type type) {
    // Store pointer to controller
    this->controller = controller;
    
    // Store type
    this->type = type;
    
    // Obviously key dependent
    keyDependent = true;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
}

UnitKeyInfo::~UnitKeyInfo() {
    delete[] output;
}

void UnitKeyInfo::apply(Instrument* instrument) {
    KeyEvent* keyEvent = instrument->currentKey;
    double frequency, velocity, duration, release, t;
    
    switch(type) {
        case Frequency:
            frequency = keyEvent->frequency;
            for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
                output[x] = frequency;
            break;
            
        case Velocity:
            velocity = keyEvent->velocity;
            for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
                output[x] = velocity;
            break;
            
        case Duration:
            duration = keyEvent->duration;
            t = (keyEvent->stage == KeyEvent::Press || keyEvent->stage == KeyEvent::Sustain) ? 1.0 / controller->getSampleRate() : 0.0;
            for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
                output[x] = duration + t * x;
            break;
            
        case Release:
            release = keyEvent->release;
            t = (keyEvent->stage == KeyEvent::Released) ? 1.0 / controller->getSampleRate() : 0.0;
            for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
                output[x] = release + t * x;
            break;
    }
}
