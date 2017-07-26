#include "nodeaudioinput.hpp"
#include "controller.hpp"
#include "audiodevices.hpp"
#include "nodeoutput.hpp"
#include "options.hpp"

NodeAudioInput::NodeAudioInput(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "audio_input";
    
    // Set options
    channel = options.getInteger("channel", 0); // TODO: start at channel 0 or at channel 1?
    
    // Hidden
    hidden = true;
    
    // Set outputs
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeAudioInput::apply() {
    float* output = this->output->getBuffer();
    
    // Copy input signal
    float* bufferInput = controller->getAudioDevices()->getBufferInput();
    int channelCount = controller->getAudioDevices()->getInputChannelCount();
    for(int x = 0;x < framesPerBuffer; ++x) {
        output[x] = bufferInput[x * channelCount + channel];
    }
}
