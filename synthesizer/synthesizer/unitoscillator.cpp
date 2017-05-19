#include "unitoscillator.hpp"
#include "controller.hpp"
#include "sample.hpp"

UnitOscillator::UnitOscillator(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Set default values
    Unit::set(controller, &frequency, "0.0");
    Unit::set(controller, &amplitude, "1.0");
    Unit::set(controller, &mean, "0.0");
    sample = Sample::sine;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
}

UnitOscillator::~UnitOscillator() {
    delete[] output;
}

void UnitOscillator::apply(Instrument* instrument) {
    frequency->update(instrument);
    amplitude->update(instrument);
    mean->update(instrument);
    
    double t = 1.0 / controller->getSampleRate();
    
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x) {
        output[x] = mean->output[x] + amplitude->output[x] * sample->getValue(phase);
        phase += t * frequency->output[x] * 2.0 * M_PI;
    }
}

bool UnitOscillator::setValue(std::string parameter, std::string value) {
    if(parameter.compare("sample") == 0) {
        Sample* s = Sample::fromString(value);
        if(s == NULL) return false;
        
        sample = s;
        return true;
    }
    
    if(parameter.compare("frequency") == 0)
        return Unit::set(controller, &frequency, value);
    
    if(parameter.compare("amplitude") == 0)
        return Unit::set(controller, &amplitude, value);
    
    if(parameter.compare("mean") == 0)
        return Unit::set(controller, &mean, value);
    
    return false;
}
