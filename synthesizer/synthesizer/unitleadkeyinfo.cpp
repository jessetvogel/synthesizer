#include "unitleadkeyinfo.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "midistate.hpp"

UnitLeadKeyInfo::UnitLeadKeyInfo(Controller* controller, InfoType infoType) : Unit(controller) {
    // Set type
    type = "lead_key_info";
    
    // Store info type
    this->infoType = infoType;
    
    // Not key dependent
    keyDependent = false;
}

void UnitLeadKeyInfo::apply(Instrument* instrument) {
    KeyEvent keyEvent = controller->getMidiState()->leadKey;
    double frequency, velocity, duration, release, t, pressing;
    
    switch(infoType) {
        case Frequency:
            frequency = keyEvent.frequency;
            for(int x = 0;x < framesPerBuffer; ++x)
                output[x] = frequency;
            break;
            
        case Velocity:
            velocity = keyEvent.velocity;
            for(int x = 0;x < framesPerBuffer; ++x)
                output[x] = velocity;
            break;
            
        case Duration:
            duration = keyEvent.duration;
            t = (keyEvent.stage == KeyEvent::Press || keyEvent.stage == KeyEvent::Sustain) ? 1.0 / sampleRate : 0.0;
            for(int x = 0;x < framesPerBuffer; ++x)
                output[x] = duration + t * x;
            break;
            
        case Release:
            release = keyEvent.release;
            t = (keyEvent.stage == KeyEvent::Released) ? 1.0 / sampleRate : 0.0;
            for(int x = 0;x < framesPerBuffer; ++x)
                output[x] = release + t * x;
            break;
            
        case Pressing:
            pressing = keyEvent.stage == KeyEvent::Press ? 1.0 : 0.0;
            for(int x = 0;x < framesPerBuffer; ++x)
                output[x] = pressing;
            break;
    }
}
