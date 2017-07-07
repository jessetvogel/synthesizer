#include "unitaudioinput.hpp"
#include "controller.hpp"
#include "audiodevices.hpp"
#include "arguments.hpp"

UnitAudioInput::UnitAudioInput(Controller* controller, Arguments arguments) : Unit(controller) {
    // Set type
    type = "audio_input";
    
    // Set arguments
    channel = arguments.getInteger("channel", 0); // TODO: start at channel 0 or at channel 1?
    
    // Hidden
    hidden = true;
}

void UnitAudioInput::apply() {
    // Copy input signal
    float* bufferInput = controller->getAudioDevices()->getBufferInput();
    int channelCount = controller->getAudioDevices()->getInputChannelCount();
    for(int x = 0;x < framesPerBuffer; ++x) {
        output[x] = bufferInput[x * channelCount + channel];
    }
}
