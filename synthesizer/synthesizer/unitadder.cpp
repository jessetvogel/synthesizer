#include "unitadder.hpp"
#include "controller.hpp"
#include "parameter.hpp"
#include "arguments.hpp"

const int UnitAdder::maxN = 16;

UnitAdder::UnitAdder(Controller* controller, Arguments arguments) : Unit(controller) {
    // Set arguments
    n = arguments.getInteger("n", 1);
    keyDependent = arguments.getBool("key", false);
    
    // Set type
    type = "adder";
    
    // Create arrays and set parameters
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
}

UnitAdder::~UnitAdder() {
    delete[] inputs;
    delete[] gains;
}

void UnitAdder::apply() {
    float buffer[framesPerBuffer];
    memset(buffer, 0, sizeof(float) * framesPerBuffer);
    for(int i = 0;i < n; i++) {
        Unit* input = (Unit*) (inputs[i]->pointer);
        Unit* gain = (Unit*) (gains[i]->pointer);

        for(int x = 0;x < framesPerBuffer; ++x)
            buffer[x] += input->output[x] * gain->output[x];
    }
    memcpy(output, buffer, sizeof(float) * framesPerBuffer);
}
