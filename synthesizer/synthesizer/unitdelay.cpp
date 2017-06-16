#include "unitdelay.hpp"
#include "controller.hpp"
#include "parameter.hpp"

const int UnitDelay::maxN = 16;
const double UnitDelay::maxT = 10.0;

UnitDelay::UnitDelay(Controller* controller, int n, double T) {
    // Store pointer to controller and other variables
    this->controller = controller;
    this->n = n;
    type = "delay";
    
    // Adders may or may not be key dependent
    this->keyDependent = false;
    
    // Set default values
    parameters.push_back(input = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "input", "0.0"));
    parameters.push_back(gain = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "gain", "1.0"));
    parameters.push_back(feedback = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "feedback", "0.0"));
    
    gains = new Parameter*[n];
    times = new Parameter*[n];
    for(int i = 0;i < n; i ++) {
        char strGain[12]; // Just to be safe it is 12, but probably any number above 9 would suffice
        char strTime[12];
        sprintf(strGain, "gain_%d", i+1);
        sprintf(strTime, "time_%d", i+1);
        parameters.push_back(gains[i] = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, strGain, "0.0"));
        parameters.push_back(times[i] = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, strTime, "0.0"));
    }
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
    
    memoryLength = (int) (controller->getSampleRate() * T);
    memory = new float[memoryLength];
    memset(memory, 0, sizeof(float) * memoryLength);
}

UnitDelay::~UnitDelay() {
    delete[] times;
    delete[] gains;
    delete[] memory;
}

void UnitDelay::apply(Instrument* instrument) {
    Unit* input = (Unit*) (this->input->pointer);
    Unit* gain = (Unit*) (this->gain->pointer);
    Unit* feedback = (Unit*) (this->feedback->pointer);
    
    unsigned long fpb = controller->getFramesPerBuffer();
    double sr = controller->getSampleRate();
    memmove(memory, memory + fpb, sizeof(float) * (memoryLength - fpb));
    
    float delaysBuffer[fpb];
    memset(delaysBuffer, 0, sizeof(float) * fpb);
    float* ptr = memory + memoryLength - fpb;
    for(int x = 0;x < fpb; ++x, ++ptr) {
        for(int i = 0;i < n;i ++) {
            Unit* gain = (Unit*) (gains[i]->pointer);
            Unit* time = (Unit*) (times[i]->pointer);
            
            double samples = time->output[x] * sr;
            int samplesLow = (int) samples;
            double f = samples - samplesLow;
            if(ptr - samplesLow >= memory) {
                delaysBuffer[x] += (*(ptr - samplesLow) * (1.0 - f) + *(ptr - samplesLow + 1) * f) * gain->output[x];
            }
        }
        
        output[x] = input->output[x] * gain->output[x] + delaysBuffer[x];
        *ptr = input->output[x] + delaysBuffer[x] * feedback->output[x];
    }
}
