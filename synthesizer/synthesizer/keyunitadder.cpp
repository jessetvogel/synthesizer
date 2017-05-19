#include "keyunitadder.hpp"
#include "controller.hpp"

const int KeyUnitAdder::maxN = 8;

KeyUnitAdder::KeyUnitAdder(Controller* controller, int n) {
    // Store pointer to controller and other variables
    this->controller = controller;
    this->n = n;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    inputs = new KeyUnit*[n];
    gains = new KeyUnit*[n];
    
    // Set default values
    for(int i = 0;i < n; i ++) {
        gains[i] = NULL;
        inputs[i] = NULL;
        KeyUnit::set(controller, &(gains[i]), "1.0");
        KeyUnit::set(controller, &(inputs[i]), "0.0");
    }
}

KeyUnitAdder::~KeyUnitAdder() {
    delete[] output;
    delete[] inputs;
    delete[] gains;
}

void KeyUnitAdder::apply(Instrument* instrument) {
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
    for(int i = 0;i < n; i++) {
        inputs[i]->update(instrument);
        gains[i]->update(instrument);
        
        for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
            output[x] += inputs[i]->output[x] * gains[i]->output[x];
    }
}

bool KeyUnitAdder::setValue(std::string parameter, std::string value) {
    for(int i = 0;i < n;i ++) {
        char strInput[12]; // Just to be safe it is 12, but probably any number above 9 would suffice
        char strGain[12];
        sprintf(strInput, "input_%d", i+1);
        sprintf(strGain, "gain_%d", i+1);
        if(parameter.compare(strInput) == 0)
            return KeyUnit::set(controller, &(inputs[i]), value);
        
        if(parameter.compare(strGain) == 0)
            return KeyUnit::set(controller, &(gains[i]), value);
    }
    
    return false;
}
