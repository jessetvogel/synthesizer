#include "unitaudiooutput.hpp"
#include "arguments.hpp"
#include "parameter.hpp"

UnitAudioOutput::UnitAudioOutput(Controller* controller, Arguments arguments) : Unit(controller) {
    // Set type
    type = "audio_output";
    
    // Set arguments
    channelCount = arguments.getInteger("channels", 1);
    
    // Create arrays and set parameters
    channels = new Parameter*[channelCount];
    for(int i = 0;i < channelCount; i ++) {
        char strChannel[12]; // Just to be safe it is 12, but probably any number above 9 would suffice
        sprintf(strChannel, "channel_%d", i+1);
        parameters.push_back(channels[i] = new Parameter(controller, Parameter::UNIT_KEY_INDEPENDENT, strChannel, "0.0"));
    }
}

UnitAudioOutput::~UnitAudioOutput() {
    delete[] channels;
}

Unit* UnitAudioOutput::getChannel(int channel) {
    Unit* unit = (Unit*) (channels[channel % channelCount]->pointer);
    return unit;
}
