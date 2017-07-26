#include "nodeADSR.hpp"
#include "controller.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "curve.hpp"
#include "midistate.hpp"
#include "options.hpp"

NodeADSR::NodeADSR(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "ADSR";
    
    // Set options
    keyNode = options.getBool("key", false);
    
    // Set inputs and outputs
    addInput("start_level", startLevel = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    addInput("attack_level", attackLevel = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "1.0"));
    addInput("sustain_level", sustainLevel = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "1.0"));
    addInput("release_level", releaseLevel = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    
    addInput("attack_time", attackTime = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    addInput("decay_time", decayTime = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    addInput("release_time", releaseTime = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    
    addInput("duration", duration = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, keyNode ? "key_duration" : "lead_key_duration"));
    addInput("release", release = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, keyNode ? "key_release" : "lead_key_release"));
    
    addInput("attack_curve", attackCurve = new NodeInput(controller, NodeInput::CURVE, "linear"));
    addInput("decay_curve", decayCurve = new NodeInput(controller, NodeInput::CURVE, "linear"));
    addInput("release_curve", releaseCurve = new NodeInput(controller, NodeInput::CURVE, "linear"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeADSR::apply() {
    float* startLevel = ((NodeOutput*) this->startLevel->pointer)->getBuffer();
    float* attackLevel = ((NodeOutput*) this->attackLevel->pointer)->getBuffer();
    float* sustainLevel = ((NodeOutput*) this->sustainLevel->pointer)->getBuffer();
    float* releaseLevel = ((NodeOutput*) this->releaseLevel->pointer)->getBuffer();
    float* attackTime = ((NodeOutput*) this->attackTime->pointer)->getBuffer();
    float* decayTime = ((NodeOutput*) this->decayTime->pointer)->getBuffer();
    float* releaseTime = ((NodeOutput*) this->releaseTime->pointer)->getBuffer();
    float* duration = ((NodeOutput*) this->duration->pointer)->getBuffer();
    float* release = ((NodeOutput*) this->release->pointer)->getBuffer();
    Curve* attackCurve = (Curve*) (this->attackCurve->pointer);
    Curve* decayCurve = (Curve*) (this->decayCurve->pointer);
    Curve* releaseCurve = (Curve*) (this->releaseCurve->pointer);
    
    float* output = this->output->getBuffer();
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        
        double attack = attackTime[x];
        double decay = decayTime[x];
        double releaseT = releaseTime[x];
        
        double d = duration[x];
        double r = release[x];
        
        float amplitude;
        
        // Attack stage
        if(d < attack)
            amplitude = Curve::ease(startLevel[x], attackLevel[x], d / attack, attackCurve);
        
        // Decay stage
        else if(d < attack + decay)
            amplitude = Curve::ease(attackLevel[x], sustainLevel[x], (d - attack) / decay, decayCurve);
        
        // Sustain stage
        else amplitude = sustainLevel[x];
        
        // Release stage
        if(r > 0) { // TODO: better way to see this?
            if(r >= releaseT)
                amplitude = releaseLevel[x];
            else
                amplitude = Curve::ease(amplitude, releaseLevel[x], r / releaseT, releaseCurve);
        }
        
        output[x] = amplitude;
    }
}
