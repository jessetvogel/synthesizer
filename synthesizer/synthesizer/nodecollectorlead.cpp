#include "nodecollectorlead.hpp"
#include "controller.hpp"
#include "keystate.hpp"
#include "voice.hpp"
#include "nodes.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "options.hpp"

NodeCollectorLead::NodeCollectorLead(Controller* controller, Options options) : NodeCollector(controller, options) {
    // Set type
    type = "collector_lead";
    
    // Set inputs and outputs
    addInput("release_time", releaseTime = new NodeInput(controller, NodeInput::NODE, "0.0"));
    
    // Create new voice
    voice = new Voice();
}

NodeCollectorLead::~NodeCollectorLead() {
    delete voice;
}

void NodeCollectorLead::addKeyEvent(KeyEvent* keyEvent) {
    KeyState* keyState = controller->getKeyState();
    switch(keyEvent->type) {
        case KeyEvent::Pressed:
            // Use this key as lead
            voice->key = keyEvent->key;
            voice->stage = Voice::Press;
            voice->release = 0.0;
            break;
            
        case KeyEvent::Released:
            if(keyEvent->key == voice->key) {
                // If other keys are pressed, use that as lead
                double minDuration = 99999.0; // TODO: less magic..
                for(int i = 0;i < AMOUNT_OF_KEYS; ++i) {
                    if(keyState->duration[i] > 0.0 && keyState->duration[i] < minDuration) {
                        voice->key = i;
                        voice->stage = Voice::Press;
                        voice->release = 0.0;
                        minDuration = keyState->duration[i];
                    }
                }
            }
            break;
    }
}

void NodeCollectorLead::apply() {
    Settings* settings = controller->getSettings();
    KeyState* keyState = controller->getKeyState();
    Nodes* nodes = controller->getNodes();
    
    // Update release
    if(voice->stage == Voice::Released)
        voice->release += settings->bufferSize / settings->sampleRate;
    
    // Update stage
    if((voice->stage == Voice::Press || voice->stage == Voice::Sustain) && keyState->velocity[voice->key] == 0.0) {
        if(keyState->sustainPedal < 0.5)
            voice->stage = Voice::Released;
        else
            voice->stage = Voice::Sustain;
    }
    
    // Update duration
    if(voice->stage == Voice::Press || voice->stage == Voice::Sustain)
        voice->duration = keyState->duration[voice->key];
    
    if(voice->stage == Voice::Released && voice->release >= getReleaseTime())
        voice->stage = Voice::Off;
    
    // Update frequency
    voice->frequency = keyState->frequency[voice->key];
    
    // Update velocity only if pressing
    if(voice->stage == Voice::Press)
        voice->velocity = keyState->velocity[voice->key];
    
    if(voice->stage != Voice::Off) {
        // Set voice and reset voice dependent nodes
        nodes->currentVoice = voice;
        nodes->resetNodesVoiceDependent();
        
        // Update node on which input depends
        input->pointer->getNode()->update();
        
        // Set output
        float* input = this->input->pointer->getBuffer();
        float* output = this->output->getBuffer();
        for(int x = 0;x < framesPerBuffer; ++x)
            output[x] = input[x];
    }
    else {
        memset(output->getBuffer(), 0, sizeof(float) * framesPerBuffer);
    }
}

double NodeCollectorLead::getReleaseTime() {
    return releaseTime->pointer->getBuffer()[0];
}
