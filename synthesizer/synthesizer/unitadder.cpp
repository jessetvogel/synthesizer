#include "unitadder.hpp"
#include "controller.hpp"

const int UnitAdder::maxN = 8;

UnitAdder::UnitAdder(Controller* controller, bool keyDependent, int n) {
    // Store pointer to controller and other variables
    this->controller = controller;
    this->n = n;
    
    // Adders may or may not be key dependent
    this->keyDependent = keyDependent;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    inputs = new Unit*[n];
    gains = new Unit*[n];
    
    // Set default values
    for(int i = 0;i < n; i ++) {
        gains[i] = NULL;
        inputs[i] = NULL;
        Unit::set(controller, &(gains[i]), "1.0", keyDependent);
        Unit::set(controller, &(inputs[i]), "0.0", keyDependent);
    }
}

UnitAdder::~UnitAdder() {
    delete[] output;
    delete[] inputs;
    delete[] gains;
}

void UnitAdder::apply(Instrument* instrument) {
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
    for(int i = 0;i < n; i++) {
        inputs[i]->update(instrument);
        gains[i]->update(instrument);

        for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
            output[x] += inputs[i]->output[x] * gains[i]->output[x];
        }
}

bool UnitAdder::setValue(std::string parameter, std::string value) {
    for(int i = 0;i < n;i ++) {
        char strInput[12]; // Just to be safe it is 12, but probably any number above 9 would suffice
        char strGain[12];
        sprintf(strInput, "input_%d", i+1);
        sprintf(strGain, "gain_%d", i+1);
        if(parameter.compare(strInput) == 0)
            return Unit::set(controller, &(inputs[i]), value, keyDependent);
        
        if(parameter.compare(strGain) == 0)
            return Unit::set(controller, &(gains[i]), value, keyDependent);
    }
    
    return false;
}
