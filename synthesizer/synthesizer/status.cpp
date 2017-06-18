#include <iostream>
#include <sstream>

#include "status.hpp"
#include "controller.hpp"
#include "mididevices.hpp"
#include "audiodevices.hpp"
#include "units.hpp"
#include "unit.hpp"
#include "parameter.hpp"

#include "error.hpp"

Status::Status(Controller* controller) {
    // Save pointer to controller
    this->controller = controller;
}

bool Status::print(std::string info) {
    
    std::istringstream iss(info);
    std::string token;
    bool error = false;
    while(std::getline(iss, token, ' ')) {
        
        if(token.compare("midi_devices") == 0) {
            controller->getMIDIDevices()->printMIDIDevices();
            continue;
        }
        
        if(token.compare("input_devices") == 0) {
            controller->getAudioDevices()->printInputDevices();
            continue;
        }

        if(token.compare("output_devices") == 0) {
            controller->getAudioDevices()->printOutputDevices();
            continue;
        }

        if(token.compare("units") == 0) {
            controller->getUnits()->printUnits();
            continue;
        }
        
        if(!error) {
            Error::addError(Error::INVALID_ARGUMENT);
            error = true;
        }
    }
    
    return true;
}

void MIDIDevices::printMIDIDevices() {
    
    std::cout << "\"midiDevices\": [";
    
    int amountOfDevices = MIDIDevices::amountOfDevices();
    bool comma = false;
    for(int i = 0;i < amountOfDevices;i ++) {
        if(MIDIDevices::isInput(i)) {
            MIDIDevice* midiDevice = controller->getMIDIDevices()->get(i);
            bool active = (midiDevice != NULL);
            
            if(comma) std::cout << ","; else comma = true; // Make sure devices get separated by commas
            std::cout << "{";
            
            std::cout << "\"id\": " << i << ", ";
            std::cout << "\"name\": \"" << MIDIDevices::deviceName(i) << "\", ";
            std::cout << "\"active\": " << (active ? "true" : "false");
            
            std::cout << "}";
        }
    }
    
    std::cout << "],";
    
}
void AudioDevices::printInputDevices() {
    
    std::cout << "\"inputDevices\": [";
    
    int amountOfDevices = AudioDevices::amountOfDevices();
    bool comma = false;
    for(int i = 0;i < amountOfDevices;i ++) {
        if(AudioDevices::isInput(i)) {
            if(comma) std::cout << ","; else comma = true; // Make sure devices get separated by commas
            std::cout << "{";
            
            std::cout << "\"id\": " << i << ", ";
            std::cout << "\"name\": \"" << AudioDevices::deviceName(i) << "\"";
            
            std::cout << "}";
        }
    }
    
    std::cout << "],";
    
}

void AudioDevices::printOutputDevices() {
    
    std::cout << "\"outputDevices\": [";
    
    int amountOfDevices = AudioDevices::amountOfDevices();
    bool comma = false;
    for(int i = 0;i < amountOfDevices;i ++) {
        if(AudioDevices::isOutput(i)) {
            if(comma) std::cout << ","; else comma = true; // Make sure devices get separated by commas
            std::cout << "{";
            
            std::cout << "\"id\": " << i << ", ";
            std::cout << "\"name\": \"" << AudioDevices::deviceName(i) << "\"";
            
            std::cout << "}";
        }
    }
    
    std::cout << "],";
    
}

void Units::printUnits() {
    
    std::cout << "\"units\": [";
    
    bool comma = false;
    for(auto it = units.begin(); it != units.end(); ++it) {
        Unit* unit = *it;
        if(comma) std::cout << ","; else comma = true; // Make sure units get separated by commas
        std::cout << "{";
        
        std::cout << "\"id\": \"" << unit->getId() << "\", ";
        std::cout << "\"type\": \"" << unit->getType() << "\", ";
        std::cout << "\"keyDependent\": \"" << (unit->isKeyDependent() ? "true" : "false") << "\", ";
        std::cout << "\"parameters\": [";
        
        bool comma_p = false;
        for(auto it_p = unit->parameters.begin(); it_p != unit->parameters.end(); ++it_p) {
            Parameter* parameter = *it_p;
            
            if(comma_p) std::cout << ","; else comma_p = true; // Make sure parameters get separated by commas
            std::cout << "{";
            
            std::cout << "\"label\": \"" << parameter->label << "\", ";
            std::cout << "\"type\": \"" << Parameter::typeToString(parameter->type) << "\", ";
            std::cout << "\"value\": \"" << parameter->valueToString() << "\"";
            
            std::cout << "}";
        }
        
        std::cout << "]";
        
        std::cout << "}";
    }
    
    std::cout << "],";
    
}
