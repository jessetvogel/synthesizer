#include "audiodevices.hpp"
#include "controller.hpp"
#include "settings.hpp"

#include "status.hpp"

const int AudioDevices::DEVICE_ID_DEFAULT = -1;
const int AudioDevices::DEVICE_ID_NONE = -2;

AudioDevices::AudioDevices(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
    
    // Set default values
    active = false;
    setInputDeviceId(DEVICE_ID_NONE);
    setOutputDeviceId(DEVICE_ID_DEFAULT);
    inputChannelCount = 1;
    outputChannelCount = 1;
}

bool AudioDevices::setInputDeviceId(int n) {
    bool wasActive = active;
    if(active) controller->stop();
    
    int deviceId;
    switch(n) {
        case DEVICE_ID_NONE:
            inputDeviceId = DEVICE_ID_NONE;
            return true;
            
        case DEVICE_ID_DEFAULT:
            deviceId = Pa_GetDefaultInputDevice();
            break;
            
        default:
            deviceId = n;
            break;
    }
    
    if(!isInput(deviceId)) return false;
    inputDeviceId = deviceId;
    
    if(wasActive) controller->start();
    return true;
}

bool AudioDevices::setOutputDeviceId(int n) {
    bool wasActive = active;
    if(active) controller->stop();
    
    int deviceId;
    switch(n) {
        case DEVICE_ID_DEFAULT:
            deviceId = Pa_GetDefaultOutputDevice();
            break;
            
        default:
            deviceId = n;
            break;
    }
    
    if(!isOutput(deviceId)) return false;
    outputDeviceId = deviceId;
    
    if(wasActive) controller->start();
    return true;
}

bool AudioDevices::start() {
    if(active) {
        Status::addError("Stream already started");
        return false;
    }
    
    PaStreamParameters inputParameters;
    PaStreamParameters outputParameters;
    auto inputParametersAddr = &inputParameters;
    auto outputParametersAddr = &outputParameters;

    const PaDeviceInfo* infoOutputDevice = Pa_GetDeviceInfo(outputDeviceId);
    if(infoOutputDevice == NULL) {
        Status::addError("Output device does not exist");
        return false;
    }
    
    bzero(&outputParameters, sizeof(outputParameters));
    outputParameters.channelCount = outputChannelCount = infoOutputDevice->maxOutputChannels;
    outputParameters.device = outputDeviceId;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = infoOutputDevice->defaultLowOutputLatency ;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    
    if(inputDeviceId != DEVICE_ID_NONE) {
        const PaDeviceInfo* infoInputDevice = Pa_GetDeviceInfo(inputDeviceId);
        if(infoInputDevice == NULL) {
            Status::addError("Input device does not exist");
            return false;
        }
        
        inputParameters.channelCount = inputChannelCount = infoInputDevice->maxInputChannels;
        inputParameters.device = inputDeviceId;
        inputParameters.sampleFormat = paFloat32;
        inputParameters.suggestedLatency = infoInputDevice->defaultLowInputLatency;
        inputParameters.hostApiSpecificStreamInfo = NULL;
    }
    else {
        inputParametersAddr = NULL;
    }
    
    PaError err = Pa_OpenStream(
                        &stream,
                        inputParametersAddr,
                        outputParametersAddr,
                        controller->getSettings()->sampleRate,
                        controller->getSettings()->bufferSize,
                        paClipOff,
                        callback,
                        (void*) this);
    
    if(err != paNoError) {
        Status::addError("Failed to open stream");
        return false;
    }
    
    // Create new buffers
    unsigned long framesPerBuffer = controller->getSettings()->bufferSize;
    bufferInput = new float[inputChannelCount * framesPerBuffer];
    bufferOutput = new float[outputChannelCount * framesPerBuffer];
    memset(bufferInput, 0, sizeof(float) * inputChannelCount * framesPerBuffer);
    memset(bufferOutput, 0, sizeof(float) * outputChannelCount * framesPerBuffer);
    
    err = Pa_StartStream(stream);

    if(err != paNoError) {
        Status::addError("Failed to start stream");
        return false;
    }
    
    active = true;
    return true;
}

bool AudioDevices::stop() {
    if(!active) {
        Status::addError("Stream was not yet started");
        return false;
    }
    
    PaError err = Pa_CloseStream(stream);
    if(err != paNoError) {
        Status::addError("Failed to close stream");
        return false;
    }

    // Deallocate buffers
    delete[] bufferInput;
    delete[] bufferOutput;
    
    active = false;
    return true;
}

int AudioDevices::callback(const void* inputBuffer,
                        void* outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void* userData) {
    // Prevent unused variables warnings
    (void) timeInfo;
    (void) statusFlags;
    
    // Get output object
    AudioDevices* audioDevices = (AudioDevices*) userData;
    Controller* controller = audioDevices->controller;

    // Store input, if input device was set
    if(audioDevices->inputDeviceId != DEVICE_ID_NONE) {
        float* in = (float*) inputBuffer;
        float* buffer = audioDevices->bufferInput;
        for(int x = 0;x < framesPerBuffer; ++x) {
            for(int channel = 0;channel < audioDevices->inputChannelCount; ++channel)
                buffer[audioDevices->inputChannelCount * x + channel] = *(in++);
        }
    }
    
    // Update everything
    float* out = (float*) outputBuffer;
    if(controller->update()) {
        // Output the buffer
        float* buffer = audioDevices->bufferOutput;
        for(int x = 0;x < framesPerBuffer; ++x) {
            for(int channel = 0;channel < audioDevices->outputChannelCount; ++channel)
                *(out++) = buffer[audioDevices->outputChannelCount * x + channel];
        }
        return paContinue;
    }
    else {
        // On failure, send silence and abort
        for(int i = 0;i < framesPerBuffer;i ++) {
            for(int channel = 0;channel < audioDevices->outputChannelCount; ++channel)
                *(out++) = 0.0;
        }
        return paAbort;
    }
}

int AudioDevices::amountOfDevices() {
    return Pa_GetDeviceCount();
}

const char* AudioDevices::deviceName(int n) {
    const PaDeviceInfo* info = Pa_GetDeviceInfo(n);
    if(info == NULL) {
        Status::addError("Output device does not exist");
        return NULL;
    }
    return Pa_GetDeviceInfo(n)->name;
}

bool AudioDevices::isInput(int n) {
    const PaDeviceInfo* info = Pa_GetDeviceInfo(n);
    if(info == NULL) return false;
    return Pa_GetDeviceInfo(n)->maxInputChannels > 0;
}

bool AudioDevices::isOutput(int n) {
    const PaDeviceInfo* info = Pa_GetDeviceInfo(n);
    if(info == NULL) return false;
    return Pa_GetDeviceInfo(n)->maxOutputChannels > 0;
}
