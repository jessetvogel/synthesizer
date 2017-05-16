#include "controller.hpp"
#include <iostream>
#include "log.hpp"

Controller::Controller() {
    // Create instances
    input = new Input(this);
    output = new Output(this);
    midiState = new MidiState();
    settings = new Settings();
    
    // Set default values
    active = false;
    inputDevice = -1;
    outputDevice = -1;
    sampleRate = 44100;
    framesPerBuffer = 64;
}

Controller::~Controller() {
    delete input;
    delete output;
    delete midiState;
    delete settings;
    
    for(auto it = instruments.begin(); it != instruments.end(); ++it)
        delete (*it);
}

void Controller::start() {
    if(active) {
        Log::warning("Tried to start Controller, but already started");
        return;
    }
    
    buffer = new float[framesPerBuffer];
    
    input->start();
    output->start();
        
    active = true;
}

void Controller::stop() {
    if(!active) {
        Log::warning("Tried to stop Controller, but wasn't started yet");
    }
    
    output->stop();
    input->stop();
    
    delete[] buffer;
    
    active = false;
}

float* Controller::update() {
    input->update();

    // TODO: something with nodes or so?
    
    memset(buffer, 0, sizeof(float) * framesPerBuffer);
    
    for(auto it = instruments.begin(); it != instruments.end(); ++it) {
        (*it)->update(midiState);
        (*it)->addBuffer(buffer);
    }
    
    return buffer;
}

MidiState* Controller::getMidiState() {
    return midiState;
}

Settings* Controller::getSettings() {
    return settings;
}

bool Controller::setInputDevice(int n) {
    if(active)
        return false;
    
    inputDevice = n;
    return true;
}

bool Controller::setOutputDevice(int n) {
    if(active)
        return false;
    
    outputDevice = n;
    return true;
    
}

bool Controller::setSampleRate(double sampleRate) {
    if(active)
        return false;
    
    this->sampleRate = sampleRate;
    return true;
    
}

bool Controller::setFramesPerBuffer(unsigned long framesPerBuffer) {
    if(active)
        return false;
    
    this->framesPerBuffer = framesPerBuffer;
    return true;
    
}

int Controller::getInputDevice() {
    return inputDevice;
}

int Controller::getOutputDevice() {
    return outputDevice;
}

double Controller::getSampleRate() {
    return sampleRate;
}

unsigned long Controller::getFramesPerBuffer() {
    return framesPerBuffer;
}

void Controller::listInputDevices() {
    int N = input->amountOfDevices();
    for(int n = 0;n < N;n ++) {
        if(input->isInput(n))
            std::cout << "(" << n << ") " << input->deviceName(n) << std::endl;
    }
}

void Controller::listOutputDevices() {
    int N = output->amountOfDevices();
    for(int n = 0;n < N;n ++) {
        if(output->isOutput(n))
            std::cout << "(" << n << ") " << output->deviceName(n) << std::endl;
    }
}

void Controller::addInstrument(Instrument* instrument) {
    instruments.push_back(instrument);
}
