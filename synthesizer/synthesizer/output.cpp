#include <portaudio.h>

#include "output.hpp"
#include "controller.hpp"

#include "log.hpp"

Output::Output(Controller* controller) {
    // Store pointer to controller object
    this->controller = controller;
    
    // Set default values
    active = false;
}

int Output::amountOfDevices() {
    return Pa_GetDeviceCount();
}

const char* Output::deviceName(int n) {
    // TODO: maybe check if it is null
    return Pa_GetDeviceInfo(n)->name;
}

bool Output::isOutput(int n) {
    return Pa_GetDeviceInfo(n)->maxOutputChannels > 0;
}

void Output::start() {
    if(active) {
        Log::warning("Output already started");
        return;
    }
    
    int outputDevice = controller->getOutputDevice();
    
    if(outputDevice == -1)
        outputDevice = Pa_GetDefaultOutputDevice();
    
    const PaDeviceInfo* info = Pa_GetDeviceInfo(outputDevice);
    if(info == NULL) {
        Log::error("Failed to open output device");
        return;
    }
    
    PaStreamParameters outputParameters;
    bzero(&outputParameters, sizeof(outputParameters));
    outputParameters.channelCount = info->maxOutputChannels;
    outputParameters.device = outputDevice;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = info->defaultLowOutputLatency ;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    PaError err = Pa_OpenStream(
                        &outputStream,
                        NULL,
                        &outputParameters,
                        controller->getSampleRate(),
                        controller->getFramesPerBuffer(),
                        paNoFlag,
                        callback,
                        (void*) this);
    
    err = Pa_StartStream(outputStream);
    
    active = true;
}

void Output::stop() {
    if(!active) {
        Log::warning("Tried to stop stream, but it wasn't running");
        return;
    }
    
//    PaError err = // TODO: check for errors
    Pa_CloseStream(outputStream);
}

int Output::callback(const void* inputBuffer,
                        void* outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void* userData) {
    // Prevent unused variables warnings
    (void) inputBuffer;
    (void) timeInfo;
    (void) statusFlags;

    // Get Output object
    Output* output = (Output*) userData;
    float* buffer = output->controller->update();
    
    float* out = (float*) outputBuffer;
    for(int i = 0;i < framesPerBuffer;i ++) {
        *(out++) = buffer[i];
        *(out++) = buffer[i];
    }
    
    return paContinue;
}
