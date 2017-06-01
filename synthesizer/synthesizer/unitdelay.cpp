#include "unitdelay.hpp"
#include "controller.hpp"

const int UnitDelay::maxN = 16;
const double UnitDelay::maxT = 10.0;

UnitDelay::UnitDelay(Controller* controller, int n, double T) {
    // Store pointer to controller and other variables
    this->controller = controller;
    this->n = n;
    
    // Adders may or may not be key dependent
    this->keyDependent = false;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    memset(output, 0, sizeof(float) * controller->getFramesPerBuffer());
    times = new Unit*[n];
    gains = new Unit*[n];
    
    memoryLength = (int) (controller->getSampleRate() * T);
    
    memory = new float[memoryLength];
    memset(memory, 0, sizeof(float) * memoryLength);
    
    // Set default values
    Unit::set(controller, &input, "0.0", keyDependent);
    Unit::set(controller, &gain, "1.0", keyDependent);
    Unit::set(controller, &feedback, "0.0", keyDependent);
    for(int i = 0;i < n; i ++) {
        gains[i] = NULL;
        times[i] = NULL;
        Unit::set(controller, &(gains[i]), "0.0", keyDependent);
        Unit::set(controller, &(times[i]), "0.0", keyDependent);
    }
}

UnitDelay::~UnitDelay() {
    delete[] output;
    delete[] times;
    delete[] gains;
    delete[] memory;
}

void UnitDelay::apply(Instrument* instrument) {
    input->update(instrument);
    gain->update(instrument);
    feedback->update(instrument);
    for(int i = 0;i < n; i++) {
        times[i]->update(instrument);
        gains[i]->update(instrument);
    }
    
    unsigned long fpb = controller->getFramesPerBuffer();
    double sr = controller->getSampleRate();
    memmove(memory, memory + fpb, sizeof(float) * (memoryLength - fpb));
    
    float delaysBuffer[fpb];
    memset(delaysBuffer, 0, sizeof(float) * fpb);
    float* ptr = memory + memoryLength - fpb;
    for(int x = 0;x < fpb; ++x, ++ptr) {
        for(int i = 0;i < n;i ++) {
            int s = (int) (times[i]->output[x] * sr);
            if(ptr - s >= memory)
                delaysBuffer[x] = (*(ptr - s)) * gains[i]->output[x];
        }
        
        output[x] = input->output[x] * gain->output[x] + delaysBuffer[x];
        *ptr = input->output[x] + delaysBuffer[x] * feedback->output[x];
    }
}

bool UnitDelay::setValue(std::string parameter, std::string value) {
    if(parameter.compare("input") == 0)
        return Unit::set(controller, &input, value, keyDependent);
    
    if(parameter.compare("gain") == 0)
        return Unit::set(controller, &gain, value, keyDependent);
    
    if(parameter.compare("feedback") == 0)
        return Unit::set(controller, &feedback, value, keyDependent);
    
    for(int i = 0;i < n;i ++) {
        char strTime[12]; // Just to be safe it is 12, but probably any number above 9 would suffice
        char strGain[12];
        sprintf(strTime, "time_%d", i+1);
        sprintf(strGain, "gain_%d", i+1);
        if(parameter.compare(strTime) == 0)
            return Unit::set(controller, &(times[i]), value, keyDependent);
        
        if(parameter.compare(strGain) == 0)
            return Unit::set(controller, &(gains[i]), value, keyDependent);
    }
    
    return false;
}
