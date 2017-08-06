#include "nodedelay.hpp"
#include "controller.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "options.hpp"

const int NodeDelay::maxN = 16;
const double NodeDelay::maxT = 10.0;

NodeDelay::NodeDelay(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "delay";

    // Set options
    n = options.getInteger("n", 1);
    double T = options.getDouble("T", 1.0);
    
    // Set inputs and outputs
    addInput("input", input = new NodeInput(controller, NodeInput::NODE, options.getString("input", "0.0")));
    addInput("gain", gain = new NodeInput(controller, NodeInput::NODE, options.getString("gain", "1.0")));
    addInput("feedback", feedback = new NodeInput(controller, NodeInput::NODE, options.getString("feedback", "0.0")));
    
    gains = new NodeInput*[n];
    times = new NodeInput*[n];
    for(int i = 0;i < n; i ++) {
        char strGain[12]; // Just to be safe it is 12, but probably any number above 9 would suffice
        char strTime[12];
        sprintf(strGain, "gain_%d", i+1);
        sprintf(strTime, "time_%d", i+1);
        addInput(strGain, gains[i] = new NodeInput(controller, NodeInput::NODE, options.getString(strGain, "0.0")));
        addInput(strTime, times[i] = new NodeInput(controller, NodeInput::NODE, options.getString(strTime, "0.0")));
    }
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
    
    // Create arrays
    memoryLength = (int) (sampleRate * T);
    memory = new float[memoryLength];
    memset(memory, 0, sizeof(float) * memoryLength);
}

NodeDelay::~NodeDelay() {
    delete[] times;
    delete[] gains;
    delete[] memory;
}

void NodeDelay::apply() {
    float* input = this->input->pointer->getBuffer();
    float* gain = this->gain->pointer->getBuffer();
    float* feedback = this->feedback->pointer->getBuffer();
    
    float* output = this->output->getBuffer();
    
    memmove(memory, memory + framesPerBuffer, sizeof(float) * (memoryLength - framesPerBuffer));
    
    float delaysBuffer[framesPerBuffer];
    memset(delaysBuffer, 0, sizeof(float) * framesPerBuffer);
    float* ptr = memory + memoryLength - framesPerBuffer;
    for(int x = 0;x < framesPerBuffer; ++x, ++ptr) {
        for(int i = 0;i < n;i ++) {
            float* gain = gains[i]->pointer->getBuffer();
            float* time = times[i]->pointer->getBuffer();
            
            double samples = time[x] * sampleRate;
            int samplesLow = (int) samples;
            double f = samples - samplesLow;
            if(ptr - samplesLow >= memory) {
                delaysBuffer[x] += (*(ptr - samplesLow) * (1.0 - f) + *(ptr - samplesLow + 1) * f) * gain[x];
            }
        }
        
        output[x] = input[x] * gain[x] + delaysBuffer[x];
        *ptr = input[x] + delaysBuffer[x] * feedback[x];
    }
}
