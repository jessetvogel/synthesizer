#include <iostream>

#include "controller.hpp"
#include "input.hpp"
#include "output.hpp"
#include "settings.hpp"
#include "midistate.hpp"
#include "instrument.hpp"

#include "unit.hpp"
#include "unitkeyfrequency.hpp"
#include "unitkeyvelocity.hpp"
#include "unitmodulationwheel.hpp"
#include "unitkeyoutput.hpp"

#include "log.hpp"

Controller::Controller() {
    // Create instances
    input = new Input(this);
    output = new Output(this);
    midiState = new MidiState(this);
    settings = new Settings();
    
    // Set default values
    inputDevice = -1;
    outputDevice = -1;
    sampleRate = 44100;
    framesPerBuffer = 64;
    active = false;
    
    // Instantiate constant values
    addUnit(new UnitKeyFrequency(this), "key_frequency");
    addUnit(new UnitKeyVelocity(this), "key_velocity");
    addUnit(new UnitKeyOutput(this), "key_output");
    addUnit(new UnitModulationWheel(this), "modulation_wheel");
}

Controller::~Controller() {
    delete input;
    delete output;
    delete midiState;
    delete settings;
    
    for(auto it = instruments.begin(); it != instruments.end(); ++it)
        delete it->second;
    
    for(auto it = units.begin(); it != units.end(); ++it)
        delete it->second;
}

bool Controller::start() {
    if(active) return false;
    
    buffer = new float[framesPerBuffer];
    
    input->start();
    output->start();
        
    active = true;
    
    return true;
}

bool Controller::stop() {
    if(!active) return false;
    
    output->stop();
    input->stop();
    
    delete[] buffer;
    
    active = false;
    
    return true;
}

bool Controller::update() {
    // Update all MIDI data
    if(!input->update()) return false;

    // Reset all units (i.e. onlyKeyDependents = false)
    resetUnits(false);
    
    // Clear buffer
    memset(buffer, 0, sizeof(float) * framesPerBuffer);
    
    // Update all instruments, and add their result to the buffer
    for(auto it = instruments.begin(); it != instruments.end(); ++it) {
        it->second->update();
        
        // Add output of the instrument to our buffer
        float* instrumentBuffer = it->second->getBuffer();
        for(int x = 0;x < framesPerBuffer; ++x)
            buffer[x] += instrumentBuffer[x];
    }
        
    return true;
}

bool Controller::setInputDevice(int n) {
    if(active) return false;
    
    inputDevice = n;
    return true;
}

bool Controller::setOutputDevice(int n) {
    if(active) return false;
    
    outputDevice = n;
    return true;
    
}

bool Controller::setSampleRate(double sampleRate) {
    if(active) return false;
    
    this->sampleRate = sampleRate;
    return true;
    
}

bool Controller::setFramesPerBuffer(unsigned long framesPerBuffer) {
    if(active) return false;
    
    this->framesPerBuffer = framesPerBuffer;
    return true;
    
}

MidiState* Controller::getMidiState() { return midiState; }

Settings* Controller::getSettings() { return settings; }

int Controller::getInputDevice() { return inputDevice; }

int Controller::getOutputDevice() { return outputDevice; }

double Controller::getSampleRate() { return sampleRate; }

unsigned long Controller::getFramesPerBuffer() { return framesPerBuffer; }

// TODO: remove this at some point
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

bool Controller::addInstrument(Instrument* instrument, std::string label) {
    if(instruments.find(label) != instruments.end()) return false;
    
    instruments[label] = instrument;
    return true;
}

bool Controller::addUnit(Unit* unit, std::string label) {
    if(units.find(label) != units.end()) return false;
    
    units[label] = unit;
    return true;
}

Instrument* Controller::getInstrument(std::string label) {
    if(instruments.find(label) == instruments.end()) return NULL;
    return instruments[label];
}

Unit* Controller::getUnit(std::string label) {
    if(units.find(label) == units.end()) return NULL;
    return units[label];
}

bool Controller::deleteInstrument(std::string label) {
    if(instruments.find(label) == instruments.end()) return false;

    Instrument* instrument = instruments[label];
    delete instrument;
    instruments.erase(label);
    return true;
}

bool Controller::deleteUnit(std::string label) {
    if(units.find(label) == units.end()) return false;

    Unit* unit = units[label];
    delete unit;
    units.erase(label);
    return true;
}

void Controller::resetUnits(bool onlyKeyDepenent) {
    for(auto it = units.begin(); it != units.end(); ++it) {
        if(!onlyKeyDepenent || it->second->isKeyDependent())
            it->second->reset();
    }
}

void Controller::addKeyEvent(KeyEvent* keyEvent) {
    // Add a copy of this key event to all instruments
    for(auto it = instruments.begin(); it != instruments.end(); ++it) {
        KeyEvent* k = new KeyEvent();
        k->key = keyEvent->key;
        k->stage = keyEvent->stage;
        k->velocity = keyEvent->velocity;
        k->frequency = keyEvent->frequency;
        k->duration = keyEvent->duration;
        k->release = keyEvent->release;
        it->second->addKeyEvent(k);
    }
}

float* Controller::getBuffer() { return buffer; }
