#include "nodeleadkeyinfo.hpp"
#include "controller.hpp"
#include "midistate.hpp"
#include "nodeoutput.hpp"

NodeLeadKeyInfo::NodeLeadKeyInfo(Controller* controller, InfoType infoType) : Node(controller) {
    // Set type
    type = "lead_key_info";
    
    // Store info type
    this->infoType = infoType;
    
    // Hidden
    hidden = true;
    
    // Set outputs
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeLeadKeyInfo::apply() {
    float* output = this->output->getBuffer();
    
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
