#include "nodecollectorpoly.hpp"
#include "controller.hpp"
#include "keystate.hpp"
#include "voice.hpp"
#include "nodes.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "options.hpp"

#include <iostream>

NodeCollectorPoly::NodeCollectorPoly(Controller* controller, Options options) : NodeCollector(controller, options) {
    // Set type
    type = "collector_poly";
    
    // Set inputs and outputs
    addInput("release_time", releaseTime = new NodeInput(controller, NodeInput::NODE, "0.0"));
}

NodeCollectorPoly::~NodeCollectorPoly() {
    for(auto it = voices.begin();it != voices.end(); ++it)
        delete *it;
}

void NodeCollectorPoly::addKeyEvent(KeyEvent* keyEvent) {
    Settings* settings = controller->getSettings();
    KeyState* keyState = controller->getKeyState();
    
    Voice* voice;
    switch(keyEvent->type) {
        case KeyEvent::Pressed:
            // If we reached voice limit, remove (and delete) the first voice
            if(voices.size() >= settings->voices) {
                delete *(voices.begin());
                voices.erase(voices.begin());
            }
            
            // Create new voice
            voice = new Voice();
            voice->key = keyEvent->key;
            voice->stage = Voice::Press;
            voice->velocity = keyState->velocity[voice->key];
            voice->release = 0.0;
            voices.push_back(voice);
            break;
            
        default: break;
    }
}

void NodeCollectorPoly::apply() {
    Settings* settings = controller->getSettings();
    KeyState* keyState = controller->getKeyState();
    Nodes* nodes = controller->getNodes();
    
    memset(output->getBuffer(), 0, sizeof(float) * framesPerBuffer);
    for(auto it = voices.begin();it != voices.end(); ++it) {
        Voice* voice = *it;
        
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
        if(voice->stage == Voice::Press)
            voice->duration = keyState->duration[voice->key];
        if(voice->stage == Voice::Sustain)
            voice->duration += settings->bufferSize / settings->sampleRate;
    
        if(voice->stage == Voice::Released && voice->release >= getReleaseTime()) {
            // Delete voice and continue
            delete voice;
            voices.erase(it);
            -- it;
            continue;
        }
    
        // Update frequency
        voice->frequency = keyState->frequency[voice->key];
    
//      // Update velocity only if pressing TODO: do we want to update the velocity?
//        if(voice->stage == Voice::Press)
//            voice->velocity = keyState->velocity[voice->key];
    
        // Set voice and reset voice dependent nodes
        nodes->currentVoice = voice;
        nodes->resetNodesVoiceDependent();
        
        // Update node on which input depends
        input->pointer->getNode()->update();
        
        // Add output
        float* input = this->input->pointer->getBuffer();
        float* output = this->output->getBuffer();
        for(int x = 0;x < framesPerBuffer; ++x)
            output[x] += input[x];
    }
}

double NodeCollectorPoly::getReleaseTime() {
    return releaseTime->pointer->getBuffer()[0];
}
