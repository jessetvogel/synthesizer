#include <portaudio.h>

#include "output.hpp"
#include "controller.hpp"

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
    const PaDeviceInfo* info = Pa_GetDeviceInfo(n);
    if(info == NULL) return NULL;
    
    return Pa_GetDeviceInfo(n)->name;
}

bool Output::isOutput(int n) {
    const PaDeviceInfo* info = Pa_GetDeviceInfo(n);
    if(info == NULL) return NULL;
    
    return Pa_GetDeviceInfo(n)->maxOutputChannels > 0;
}

bool Output::start() {
    if(active) return false;
    
    int outputDevice = controller->getOutputDevice();
    
    if(outputDevice == -1)
        outputDevice = Pa_GetDefaultOutputDevice();
    
    const PaDeviceInfo* info = Pa_GetDeviceInfo(outputDevice);
    if(info == NULL) return false;
    
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
                        paClipOff,
                        callback,
                        (void*) this);
    
    err = Pa_StartStream(outputStream);
    
    active = true;
    
    return true;
}

bool Output::stop() {
    if(!active) return false;
    
//    PaError err = // TODO: check for errors
    active = false;
    Pa_CloseStream(outputStream);

    return true;
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
    
    // Get output object
    Output* output = (Output*) userData;
    
    // Update everything
    float* out = (float*) outputBuffer;
    if(output->controller->update() && output->active) {
        // Output the buffer
        float* buffer = output->controller->getBuffer();
        for(int i = 0;i < framesPerBuffer;i ++) {
            *(out++) = buffer[i];
            *(out++) = buffer[i];
        }
        return paContinue;
    }
    else {
        // On failure, send silence and abort
        for(int i = 0;i < framesPerBuffer;i ++) {
            *(out++) = 0.0;
            *(out++) = 0.0;
        }
        return paAbort;
    }
}
