#include "output.hpp"
#include "log.hpp"
#include <portaudio.h>
#include <iostream>

Output::Output(Controller* controller) {
    // Store pointer to controller object
    this->controller = controller;
    
    // Set default values
    active = false;
    outputDevice = -1;
    sampleRate = 44100;
    framesPerBuffer = 64;
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

void Output::setOutputDevice(int n) {
    if(active) {
        Log::warning("Cannot set output device while running");
        return;
    }
    
    outputDevice = n;
}

void Output::start() {
    if(active) {
        Log::warning("Output already started");
        return;
    }
    
    if(outputDevice == -1)
        outputDevice = Pa_GetDefaultOutputDevice();
    
    const PaDeviceInfo* info = Pa_GetDeviceInfo(outputDevice);
    if(info == NULL) {
        Log::error("Failed to open output device");
        return;
    }
    
    // TODO: remove this
    std::cout << "Opening output device " <<  info->name << "; inputs: " << info->maxInputChannels << "; outputs: " << info->maxOutputChannels << std::endl;
    
    PaStreamParameters outputParameters;
    bzero(&outputParameters, sizeof(outputParameters));
    outputParameters.channelCount = 2;
    outputParameters.device = outputDevice;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = info->defaultLowOutputLatency ;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    PaError err = Pa_OpenStream(
                        &outputStream,
                        NULL,
                        &outputParameters,
                        sampleRate,
                        framesPerBuffer,
                        paClipOff,
                        callback,
                        (void*) this);
    
    err = Pa_StartStream(outputStream);
    
    active = true;
}

void Output::setSampleRate(double sampleRate) {
    if(active) {
        Log::warning("Cannot change the sample rate of Output when active");
        return;
    }
    
    this->sampleRate = sampleRate;
}

void Output::setFramesPerBuffer(unsigned long framesPerBuffer) {
    if(active) {
        Log::warning("Cannot change the frames per buffer of Output when active");
        return;
    }
    
    this->framesPerBuffer = framesPerBuffer;
}

void Output::stop() {
    if(!active) {
        Log::warning("Tried to stop stream, but it wasn't running");
        return;
    }
    
//    PaError err = // TODO: check for errors
    Pa_CloseStream(outputStream);
}

int z = 0;

#include <cmath>

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
    output->controller->update();
    
    float* out = (float*) outputBuffer;
    for(int i = 0;i < framesPerBuffer;i ++) {
        *(out++) = sin((double) z / 44100.0 * 6.28 * 440.0);
        *(out++) = sin((double) z / 44100.0 * 6.28 * 440.0 * 1.5);
        z ++;
    }
    
    return paContinue;
}