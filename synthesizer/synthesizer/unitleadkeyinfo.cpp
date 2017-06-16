#include "unitleadkeyinfo.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "midistate.hpp"

UnitLeadKeyInfo::UnitLeadKeyInfo(Controller* controller, InfoType infoType) {
    // Store pointer to controller
    this->controller = controller;
    type = "lead_key_info";
    
    // Store type
    this->infoType = infoType;
    
    // Not key dependent
    keyDependent = false;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
}

void UnitLeadKeyInfo::apply(Instrument* instrument) {
    KeyEvent keyEvent = controller->getMidiState()->leadKey;
    double frequency, velocity, duration, release, t, pressing;
    
    switch(infoType) {
        case Frequency:
            frequency = keyEvent.frequency;
            for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
                output[x] = frequency;
            break;
            
        case Velocity:
            velocity = keyEvent.velocity;
            for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
                output[x] = velocity;
            break;
            
        case Duration:
            duration = keyEvent.duration;
            t = (keyEvent.stage == KeyEvent::Press || keyEvent.stage == KeyEvent::Sustain) ? 1.0 / controller->getSampleRate() : 0.0;
            for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
                output[x] = duration + t * x;
            break;
            
        case Release:
            release = keyEvent.release;
            t = (keyEvent.stage == KeyEvent::Released) ? 1.0 / controller->getSampleRate() : 0.0;
            for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
                output[x] = release + t * x;
            break;
            
        case Pressing:
            pressing = keyEvent.stage == KeyEvent::Press ? 1.0 : 0.0;
            for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
                output[x] = pressing;
            break;
    }
}
