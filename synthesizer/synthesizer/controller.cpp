#include "controller.hpp"
#include <iostream>
#include "log.hpp"

Controller::Controller() {
    // Create instances
    input = new Input(this);
    output = new Output(this);
    midiState = new MidiState();
    
    // Set default values
    active = false;
}

void Controller::start() {
    if(active) {
        Log::warning("Tried to start Controller, but already started");
        return;
    }
    
    input->start();
    output->start();
        
    active = true;
}

void Controller::stop() {
    if(!active) {
        Log::warning("Tried to stop Contorller, but wasn't started yet");
    }
    
    output->stop();
    input->stop();
    
    active = false;
}

MidiState* Controller::getMidiState() {
    return midiState;
}

void Controller::setInputDevice(int n) {
    input->setInputDevice(n);
}

void Controller::setOutputDevice(int n) {
    output->setOutputDevice(n);
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

void Controller::update() {
    input->update();
    // TODO: update nodes and instruments etc
}

Controller::~Controller() {
    delete input;
    delete output;
    delete midiState;
}