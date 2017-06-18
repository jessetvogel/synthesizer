#include "unitADSR.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "parameter.hpp"
#include "curve.hpp"
#include "midistate.hpp"

UnitADSR::UnitADSR(Controller* controller, bool keyDependent) : Unit(controller) {
    // Set type
    type = "ADSR";
    
    // An ADSR may or may not be key dependent
    this->keyDependent = keyDependent;
    
    // Set default values
    parameters.push_back(startLevel = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "start_level", "0.0"));
    parameters.push_back(attackLevel = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "attack_level", "1.0"));
    parameters.push_back(sustainLevel = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "sustain_level", "1.0"));
    parameters.push_back(releaseLevel = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "release_level", "0.0"));
    
    parameters.push_back(attackTime = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "attack_time", "0.0"));
    parameters.push_back(decayTime = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "decay_time", "0.0"));
    parameters.push_back(releaseTime = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "release_time", "0.0"));
    
    parameters.push_back(duration = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "duration", keyDependent ? "key_duration" : "lead_key_duration"));
    parameters.push_back(release = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "release", keyDependent ? "key_release" : "lead_key_release"));
    
    parameters.push_back(attackCurve = new Parameter(controller, Parameter::CURVE, "attack_curve", "linear"));
    parameters.push_back(decayCurve = new Parameter(controller, Parameter::CURVE, "decay_curve", "linear"));
    parameters.push_back(releaseCurve = new Parameter(controller, Parameter::CURVE, "release_curve", "linear"));
}

void UnitADSR::apply(Instrument* instrument) {
    Unit* startLevel = (Unit*) (this->startLevel->pointer);
    Unit* attackLevel = (Unit*) (this->attackLevel->pointer);
    Unit* sustainLevel = (Unit*) (this->sustainLevel->pointer);
    Unit* releaseLevel = (Unit*) (this->releaseLevel->pointer);
    Unit* attackTime = (Unit*) (this->attackTime->pointer);
    Unit* decayTime = (Unit*) (this->decayTime->pointer);
    Unit* releaseTime = (Unit*) (this->releaseTime->pointer);
    Unit* duration = (Unit*) (this->duration->pointer);
    Unit* release = (Unit*) (this->release->pointer);
    Curve* attackCurve = (Curve*) (this->attackCurve->pointer);
    Curve* decayCurve = (Curve*) (this->decayCurve->pointer);
    Curve* releaseCurve = (Curve*) (this->releaseCurve->pointer);
    
    for(int x = 0;x < framesPerBuffer; ++x) {
        
        double attack = attackTime->output[x];
        double decay = decayTime->output[x];
        double releaseT = releaseTime->output[x];
        
        double d = duration->output[x];
        double r = release->output[x];
        
        float amplitude;
        
        // Attack stage
        if(d < attack)
            amplitude = Curve::ease(startLevel->output[x], attackLevel->output[x], d / attack, attackCurve);
        
        // Decay stage
        else if(d < attack + decay)
            amplitude = Curve::ease(attackLevel->output[x], sustainLevel->output[x], (d - attack) / decay, decayCurve);
        
        // Sustain stage
        else amplitude = sustainLevel->output[x];
        
        // Release stage
        if(r > 0) { // TODO: better way to see this?
            if(r >= releaseT)
                amplitude = releaseLevel->output[x];
            else
                amplitude = Curve::ease(amplitude, releaseLevel->output[x], r / releaseT, releaseCurve);
        }
        
        output[x] = amplitude;
    }
}
