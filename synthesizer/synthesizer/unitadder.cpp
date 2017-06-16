#include "unitadder.hpp"
#include "controller.hpp"
#include "parameter.hpp"

const int UnitAdder::maxN = 16;

UnitAdder::UnitAdder(Controller* controller, bool keyDependent, int n) {
    // Store pointer to controller and other variables
    this->controller = controller;
    this->n = n;
    char strType[12];
    sprintf(strType, "adder_%d", n);
    type = std::string(strType);
    
    // Adders may or may not be key dependent
    this->keyDependent = keyDependent;
    
    // Set default values
    inputs = new Parameter*[n];
    gains = new Parameter*[n];
    for(int i = 0;i < n; i ++) {
        char strInput[12]; // Just to be safe it is 12, but probably any number above 9 would suffice
        char strGain[12];
        sprintf(strInput, "input_%d", i+1);
        sprintf(strGain, "gain_%d", i+1);
        parameters.push_back(inputs[i] = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, strInput, "0.0"));
        parameters.push_back(gains[i] = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, strGain, "1.0"));
    }
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
}

UnitAdder::~UnitAdder() {
    delete[] inputs;
    delete[] gains;
}

void UnitAdder::apply(Instrument* instrument) {
    float buffer[controller->getFramesPerBuffer()];
    memset(buffer, 0, sizeof(float) * controller->getFramesPerBuffer());
    for(int i = 0;i < n; i++) {
        Unit* input = (Unit*) (inputs[i]->pointer);
        Unit* gain = (Unit*) (gains[i]->pointer);

        for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
            buffer[x] += input->output[x] * gain->output[x];
    }
    memcpy(output, buffer, sizeof(float) * controller->getFramesPerBuffer());
}
