#include "nodekeyinfo.hpp"
#include "controller.hpp"
#include "nodes.hpp"
#include "nodeoutput.hpp"

NodeKeyInfo::NodeKeyInfo(Controller* controller, InfoType infoType) : Node(controller) {
    // Set type
    type = "key_info";
    
    // Store info type
    this->infoType = infoType;
    
    // Hidden
    hidden = true;
    
    // Obviously key dependent
    keyNode = true;
    
    // Set outputs
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeKeyInfo::apply() {
    float* output = this->output->getBuffer();
    
    KeyEvent* keyEvent = controller->getNodes()->currentKey;
    double frequency, velocity, duration, release, t;

    switch(infoType) {
        case Frequency:
            frequency = keyEvent->frequency;
            for(int x = 0;x < framesPerBuffer; ++x)
                output[x] = frequency;
            break;
            
        case Velocity:
            velocity = keyEvent->velocity;
            for(int x = 0;x < framesPerBuffer; ++x)
                output[x] = velocity;
            break;
            
        case Duration:
            duration = keyEvent->duration;
            t = (keyEvent->stage == KeyEvent::Press || keyEvent->stage == KeyEvent::Sustain) ? 1.0 / sampleRate : 0.0;
            for(int x = 0;x < framesPerBuffer; ++x)
                output[x] = duration + t * x;
            break;
            
        case Release:
            release = keyEvent->release;
            t = (keyEvent->stage == KeyEvent::Released) ? 1.0 / sampleRate : 0.0;
            for(int x = 0;x < framesPerBuffer; ++x)
                output[x] = release + t * x;
            break;
    }
}