#include "nodeADSR.hpp"
#include "controller.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "curve.hpp"
#include "options.hpp"

NodeADSR::NodeADSR(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "ADSR";
    
    // Must be voice dependent
    voiceDependent = true;
    
    // Set inputs and outputs
    addInput("start_level", startLevel = new NodeInput(controller, NodeInput::NODE_VOICE, options.getString("start_level", "0.0")));
    addInput("attack_level", attackLevel = new NodeInput(controller, NodeInput::NODE_VOICE, options.getString("attack_level", "1.0")));
    addInput("sustain_level", sustainLevel = new NodeInput(controller, NodeInput::NODE_VOICE, options.getString("sustain_level", "1.0")));
    addInput("release_level", releaseLevel = new NodeInput(controller, NodeInput::NODE_VOICE, options.getString("release_level", "0.0")));
    
    addInput("attack_time", attackTime = new NodeInput(controller, NodeInput::NODE_VOICE, options.getString("attack_time", "0.0")));
    addInput("decay_time", decayTime = new NodeInput(controller, NodeInput::NODE_VOICE, options.getString("decay_time", "0.0")));
    addInput("release_time", releaseTime = new NodeInput(controller, NodeInput::NODE_VOICE, options.getString("release_time", "0.0")));
    
    addInput("duration", duration = new NodeInput(controller, NodeInput::NODE_VOICE, options.getString("duration", "key_duration")));
    addInput("release", release = new NodeInput(controller, NodeInput::NODE_VOICE, options.getString("release", "key_release")));
    
    addInput("attack_curve", attackCurve = new NodeInput(controller, NodeInput::CURVE, options.getString("attack_curve", "linear")));
    addInput("decay_curve", decayCurve = new NodeInput(controller, NodeInput::CURVE, options.getString("decay_curve", "linear")));
    addInput("release_curve", releaseCurve = new NodeInput(controller, NodeInput::CURVE, options.getString("release_curve", "linear")));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeADSR::apply() {
    float* startLevel = this->startLevel->pointer->getBuffer();
    float* attackLevel = this->attackLevel->pointer->getBuffer();
    float* sustainLevel = this->sustainLevel->pointer->getBuffer();
    float* releaseLevel = this->releaseLevel->pointer->getBuffer();
    float* attackTime = this->attackTime->pointer->getBuffer();
    float* decayTime = this->decayTime->pointer->getBuffer();
    float* releaseTime = this->releaseTime->pointer->getBuffer();
    float* duration = this->duration->pointer->getBuffer();
    float* release = this->release->pointer->getBuffer();
    Curve* attackCurve = Curve::get(this->attackCurve->pointer->getBuffer()[0]);
    Curve* decayCurve = Curve::get(this->decayCurve->pointer->getBuffer()[0]);
    Curve* releaseCurve = Curve::get(this->releaseCurve->pointer->getBuffer()[0]);
    
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
        else
            amplitude = sustainLevel[x];
        
        // Release stage
        if(r > 0.0) {
            if(r >= releaseT)
                amplitude = releaseLevel[x];
            else
                amplitude = Curve::ease(amplitude, releaseLevel[x], r / releaseT, releaseCurve);
        }
        
        output[x] = amplitude;
    }
}
