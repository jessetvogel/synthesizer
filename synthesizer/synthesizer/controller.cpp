#include <iostream>

#include "controller.hpp"
#include "input.hpp"
#include "output.hpp"
#include "settings.hpp"
#include "midistate.hpp"
#include "instrument.hpp"
#include "unit.hpp"
#include "unitparameter.hpp"

#include "unitkeyinfo.hpp"
#include "unitleadkeyinfo.hpp"
#include "unitmodulationwheel.hpp"
#include "unitkeyoutput.hpp"

#include "log.hpp"

Controller::Controller(Settings* settings) {
    // Create instances
    this->settings = settings;
    midiState = new MidiState(this);
    
    // Set some values
    sampleRate = settings->sampleRate;
    framesPerBuffer = settings->bufferSize;
    active = false;
    
    // Initialize
    reset();
}

Controller::~Controller() {
    delete midiState;
    
    for(auto it = instruments.begin(); it != instruments.end(); ++it)
        delete it->second;
    
    for(auto it = units.begin(); it != units.end(); ++it)
        delete it->second;
    
    for(auto it = inputs.begin(); it != inputs.end(); ++it)
        delete (*it);
}

bool Controller::start(int outputDevice) {
    // Check if not yet started
    if(active) return false;
    
    // Check if output device is valid
    if(outputDevice == -1) outputDevice = Pa_GetDefaultOutputDevice();
    if(!(Output::isOutput(outputDevice))) return false;
    
    // Create new buffer
    buffer = new float[framesPerBuffer];
    
    // Start all inputs TODO: we can also already start these whenever they are added. Also prevents errors probably, if they are disconnected in between.
    for(auto it = inputs.begin(); it != inputs.end(); ++it)
        if(!((*it)->start())) return false;
    
    // Create new output
    output = new Output(this, outputDevice);
    if(!output->start()) return false;
    
    active = true;
    return true;
}

bool Controller::stop() {
    // Check if we did in fact were active
    if(!active) return false;
    
    // Try to stop the output stream
    if(!output->stop()) return false;
    
    // Try to stop all the inputs
    for(auto it = inputs.begin(); it != inputs.end(); ++it)
        if(!((*it)->stop())) return false;
    
    // Delete the buffer
    delete[] buffer;
    
    // Delete the output
    delete output;
    
    active = false;
    return true;
}

bool Controller::reset() {
    if(active) return false;
    
    // Clear all instruments and units
    for(auto it = instruments.begin(); it != instruments.end(); ++it)
        delete it->second;
    
    for(auto it = units.begin(); it != units.end(); ++it)
        delete it->second;
    
    instruments.clear();
    units.clear();
    
    // Instantiate constant values
    addUnit(new UnitKeyInfo(this, UnitKeyInfo::Frequency), "key_frequency");
    addUnit(new UnitKeyInfo(this, UnitKeyInfo::Velocity), "key_velocity");
    addUnit(new UnitKeyInfo(this, UnitKeyInfo::Duration), "key_duration");
    addUnit(new UnitKeyInfo(this, UnitKeyInfo::Release), "key_release");
    
    addUnit(new UnitLeadKeyInfo(this, UnitLeadKeyInfo::Frequency), "lead_key_frequency");
    addUnit(new UnitLeadKeyInfo(this, UnitLeadKeyInfo::Velocity), "lead_key_velocity");
    addUnit(new UnitLeadKeyInfo(this, UnitLeadKeyInfo::Duration), "lead_key_duration");
    addUnit(new UnitLeadKeyInfo(this, UnitLeadKeyInfo::Release), "lead_key_release");
    addUnit(new UnitLeadKeyInfo(this, UnitLeadKeyInfo::Pressing), "lead_key_pressing");
    
    addUnit(new UnitKeyOutput(this), "key_output");
    addUnit(new UnitModulationWheel(this), "modulation_wheel");
    
    return true;
}

bool Controller::update() {
    // Update all MIDI data
    for(auto it = inputs.begin(); it != inputs.end(); ++it)
        if(!((*it)->update())) return false;

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

bool Controller::addInputDevice(int n) {
    if(active) return false;
    if(!(Input::isInput(n))) return false;
    
    inputs.push_back(new Input(this, n));
    return true;
}

bool Controller::removeInputDevice(int n) {
    if(active) return false;
    
    for(auto it = inputs.end() - 1; it >= inputs.begin(); --it) {
        if((*it)->getInputDevice() == n) {
            delete *it;
            inputs.erase(it);
            return true;
        }
    }
    
    return false;
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

bool Controller::addUnitParameter(UnitParameter* parameter, int MidiCC) {
    if(parameters.find(MidiCC) != parameters.end()) return false;
    
    parameters[MidiCC] = parameter;
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

UnitParameter* Controller::getUnitParameter(int MidiCC) {
    if(parameters.find(MidiCC) == parameters.end()) return NULL;
    return parameters[MidiCC];
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

bool Controller::deleteUnitParameter(int MidiCC) {
    if(parameters.find(MidiCC) == parameters.end()) return false;
    
    UnitParameter* parameter = parameters[MidiCC];
    delete parameter;
    parameters.erase(MidiCC);
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






// TODO: remove this at some point
void Controller::listInputDevices() {
    int N = Input::amountOfDevices();
    for(int n = 0;n < N;n ++) {
        if(Input::isInput(n))
            std::cout << "(" << n << ") " << Input::deviceName(n) << std::endl;
    }
}

void Controller::listOutputDevices() {
    int N = output->amountOfDevices();
    for(int n = 0;n < N;n ++) {
        if(output->isOutput(n))
            std::cout << "(" << n << ") " << output->deviceName(n) << std::endl;
    }
}
