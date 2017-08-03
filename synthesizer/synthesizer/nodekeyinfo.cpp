#include "nodekeyinfo.hpp"
#include "controller.hpp"
#include "nodes.hpp"
#include "nodeoutput.hpp"
#include "voice.hpp"

NodeKeyInfo::NodeKeyInfo(Controller* controller, InfoType infoType) : Node(controller) {
    // Set type
    type = "key_info";
    
    // Store info type
    this->infoType = infoType;
    
    // Hidden
    hidden = true;
    
    // Obviously key dependent
    voiceDependent = true;
    
    // Set outputs
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeKeyInfo::apply() {
    float* output = this->output->getBuffer();
    
    Voice* voice = controller->getNodes()->currentVoice;
    double frequency, velocity, duration, release, t;

    switch(infoType) {
        case Frequency:
            frequency = voice->frequency;
            for(int x = 0;x < framesPerBuffer; ++x)
                output[x] = frequency;
            break;
            
        case Velocity:
            velocity = voice->velocity;
            for(int x = 0;x < framesPerBuffer; ++x)
                output[x] = velocity;
            break;
            
        case Duration:
            duration = voice->duration;
            t = (voice->stage == Voice::Press || voice->stage == Voice::Sustain) ? 1.0 / sampleRate : 0.0;
            for(int x = 0;x < framesPerBuffer; ++x)
                output[x] = duration + t * x;
            break;
            
        case Release:
            release = voice->release;
            t = (voice->stage == Voice::Released) ? 1.0 / sampleRate : 0.0;
            for(int x = 0;x < framesPerBuffer; ++x)
                output[x] = release + t * x;
            break;
    }
}
