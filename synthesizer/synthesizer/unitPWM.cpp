#include "unitPWM.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "settings.hpp"

UnitPWM::UnitPWM(Controller* controller, bool keyDependent) {
    // Store pointer to controller
    this->controller = controller;
    
    // May or may not be key dependent
    this->keyDependent = keyDependent;
    
    // Set default values
    Unit::set(controller, &frequency, "0.0", keyDependent);
    Unit::set(controller, &duty, "0.0", keyDependent);
    Unit::set(controller, &low, "0.0", keyDependent);
    Unit::set(controller, &high, "1.0", keyDependent);
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
    
    if(keyDependent) {
        phase = new double[MAX_AMOUNT_OF_IDS];
        memset(phase, 0, sizeof(double) * MAX_AMOUNT_OF_IDS);
    }
    else {
        phase = new double[1];
        phase[0] = 0.0;
    }
}

UnitPWM::~UnitPWM() {
    delete[] phase;
    delete[] output;
}

void UnitPWM::apply(Instrument* instrument) {
    frequency->update(instrument);
    duty->update(instrument);
    low->update(instrument);
    high->update(instrument);
   
    double t = 1.0 / controller->getSampleRate();
    int i = keyDependent ? instrument->currentKey->id : 0;
    
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x) {
        output[x] = phase[i] < duty->output[x] ? high->output[x] : low->output[x];
        phase[i] += t * frequency->output[x];
        if(phase[i] >= 1.0) phase[i] -= floor(phase[i]);
    }
}

bool UnitPWM::setValue(std::string parameter, std::string value) {
    if(parameter.compare("frequency") == 0)
        return Unit::set(controller, &frequency, value, keyDependent);
    
    if(parameter.compare("duty") == 0)
        return Unit::set(controller, &duty, value, keyDependent);

    if(parameter.compare("low") == 0)
        return Unit::set(controller, &low, value, keyDependent);
    
    if(parameter.compare("high") == 0)
        return Unit::set(controller, &high, value, keyDependent);
    
    return false;
}
