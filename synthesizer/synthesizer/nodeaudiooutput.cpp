#include "nodeaudiooutput.hpp"
#include "options.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"

NodeAudioOutput::NodeAudioOutput(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "audio_output";
    
    // Set options
    channelCount = options.getInteger("channels", 1);
    
    // Set inputs and outputs
    channels = new NodeInput*[channelCount];
    for(int i = 0;i < channelCount; i ++) {
        char strChannel[12]; // Just to be safe it is 12, but probably any number above 9 would suffice
        sprintf(strChannel, "channel_%d", i+1);
        addInput(strChannel, channels[i] = new NodeInput(controller, NodeInput::NODE, "0.0"));
    }
}

NodeAudioOutput::~NodeAudioOutput() {
    delete[] channels;
}

NodeOutput* NodeAudioOutput::getChannel(int channel) {
    return channels[channel % channelCount]->pointer;
}
