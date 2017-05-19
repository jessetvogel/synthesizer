#include <iostream>

#include "controller.hpp"
#include "input.hpp"
#include "output.hpp"
#include "settings.hpp"
#include "midistate.hpp"
#include "instrument.hpp"
#include "unit.hpp"
#include "keyunit.hpp"
#include "keyunitfrequency.hpp"
#include "unitmodulationwheel.hpp"

#include "log.hpp"

Controller::Controller() {
    // Create instances
    input = new Input(this);
    output = new Output(this);
    settings = new Settings();
    midiState = new MidiState(settings);
    
    // Set default values
    active = false;
    inputDevice = -1;
    outputDevice = -1;
    sampleRate = 44100;
    framesPerBuffer = 64;
    
    // Instantiate constant values
    addKeyUnit(new KeyUnitFrequency(this), "key_frequency");
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
    
    for(auto it = keyUnits.begin(); it != keyUnits.end(); ++it)
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

float* Controller::update() {
    // Update all MIDI data
    input->update();

    // Reset all units
    resetUnits();
    
    // Clear buffer
    memset(buffer, 0, sizeof(float) * framesPerBuffer);
    
    // Update all instruments, and add their result to the buffer
    for(auto it = instruments.begin(); it != instruments.end(); ++it) {
        it->second->update(midiState);
        it->second->addBuffer(buffer);
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

bool Controller::addKeyUnit(KeyUnit* keyUnit, std::string label) {
    if(keyUnits.find(label) != keyUnits.end()) return false;
    
    keyUnits[label] = keyUnit;
    return true;
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

bool Controller::deleteKeyUnit(std::string label) {
    if(keyUnits.find(label) == keyUnits.end()) return false;
    
    KeyUnit* keyUnit = keyUnits[label];
    delete keyUnit;
    keyUnits.erase(label);
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

KeyUnit* Controller::getKeyUnit(std::string label) {
    if(keyUnits.find(label) == keyUnits.end()) return NULL;
    return keyUnits[label];
}

void Controller::resetUnits() {
    for(auto it = units.begin(); it != units.end(); ++it)
        it->second->reset();
}

void Controller::resetKeyUnits() {
    for(auto it = keyUnits.begin(); it != keyUnits.end(); ++it)
        it->second->reset();
}
