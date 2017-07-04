#include <iostream>
#include <sstream>

#include "status.hpp"
#include "controller.hpp"
#include "instruments.hpp"
#include "instrument.hpp"
#include "mididevices.hpp"
#include "audiodevices.hpp"
#include "units.hpp"
#include "unit.hpp"
#include "blocks.hpp"
#include "block.hpp"
#include "parameter.hpp"

#include "error.hpp"

Status::Status(Controller* controller) {
    // Save pointer to controller
    this->controller = controller;
}

void Status::start() {
    
    std::cout << "{";
    
    std::cout << "\"info\":[\"Program started\"],";
    
    Error::printErrors();
    
    std::cout << "}" << std::endl;
    std::cout.flush();
    
}

void Status::stop() {
    
    std::cout << "{";
    
    std::cout << "\"info\":[\"Program stopped\"]";
    
    std::cout << "}" << std::endl;
    std::cout.flush();
    
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
        
        if(token.compare("instruments") == 0) {
            controller->getInstruments()->printInstruments();
            continue;
        }

        if(token.compare("units") == 0) {
            controller->getUnits()->printUnits();
            continue;
        }
        
        if(token.compare("blocks") == 0) {
            controller->getBlocks()->printBlocks();
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
    
    std::cout << "\"midiDevices\":[";
    
    int amountOfDevices = MIDIDevices::amountOfDevices();
    bool comma = false;
    for(int i = 0;i < amountOfDevices;i ++) {
        if(MIDIDevices::isInput(i)) {
            MIDIDevice* midiDevice = controller->getMIDIDevices()->get(i);
            bool active = (midiDevice != NULL);
            
            if(comma) std::cout << ","; else comma = true;
            std::cout << "{";
            
            std::cout << "\"id\":" << i << ", ";
            std::cout << "\"name\":\"" << MIDIDevices::deviceName(i) << "\",";
            std::cout << "\"active\":" << (active ? "true" : "false");
            
            std::cout << "}";
        }
    }
    
    std::cout << "],";
    
}
void AudioDevices::printInputDevices() {
    
    std::cout << "\"inputDevices\":[";
    
    int amountOfDevices = AudioDevices::amountOfDevices();
    bool comma = false;
    for(int i = 0;i < amountOfDevices;i ++) {
        if(AudioDevices::isInput(i)) {
            if(comma) std::cout << ","; else comma = true;
            std::cout << "{";
            
            std::cout << "\"id\":" << i << ", ";
            std::cout << "\"name\":\"" << AudioDevices::deviceName(i) << "\"";
            
            std::cout << "}";
        }
    }
    
    std::cout << "],";
    
}

void AudioDevices::printOutputDevices() {
    
    std::cout << "\"outputDevices\":[";
    
    int amountOfDevices = AudioDevices::amountOfDevices();
    bool comma = false;
    for(int i = 0;i < amountOfDevices;i ++) {
        if(AudioDevices::isOutput(i)) {
            if(comma) std::cout << ","; else comma = true;
            std::cout << "{";
            
            std::cout << "\"id\":" << i << ",";
            std::cout << "\"name\":\"" << AudioDevices::deviceName(i) << "\"";
            
            std::cout << "}";
        }
    }
    
    std::cout << "],";
    
}

void Instruments::printInstruments() {
    
    std::cout << "\"instruments\":[";
    
    bool comma = false;
    for(auto it = instruments.begin(); it != instruments.end(); ++it) {
        Instrument* instrument = *it;
        if(comma) std::cout << ","; else comma = true;
        
        std::cout << "{";
        
        std::cout << "\"id\":\"" << instrument->getId() << "\",";
        std::cout << "\"key_output\":\"" << ((Unit*) (instrument->getKeyOutput()->pointer))->getId() << "\",";
        std::cout << "\"output\":\"" << ((Unit*) (instrument->getOutput()->pointer))->getId() << "\"";
        
        std::cout << "}";
    }
    
    std::cout << "],";
    
}

void Units::printUnits() {
    
    std::cout << "\"units\":[";
    
    bool comma = false;
    for(auto it = units.begin(); it != units.end(); ++it) {
        Unit* unit = *it;
        if(unit->isHidden()) continue;
        
        if(comma) std::cout << ","; else comma = true;
        std::cout << "{";
        
        unit->printUnit();
        
        std::cout << "}";
    }
    
    std::cout << "],";
    
}

void Unit::printUnit() {
    
    std::cout << "\"id\":\"" << id << "\",";
    std::cout << "\"type\":\"" << type << "\",";
    std::cout << "\"keyDependent\": \"" << (keyDependent ? "true" : "false") << "\",";
    std::cout << "\"parameters\":[";
    
    bool comma = false;
    for(auto it = parameters.begin(); it != parameters.end(); ++it) {
        Parameter* parameter = *it;
        
        if(comma) std::cout << ","; else comma = true;
        std::cout << "{";
        
        std::cout << "\"label\":\"" << parameter->label << "\",";
        std::cout << "\"type\":\"" << Parameter::typeToString(parameter->type) << "\",";
        std::cout << "\"value\":\"" << parameter->valueToString() << "\"";
        
        std::cout << "}";
    }
    
    std::cout << "]";
    
}

void Blocks::printBlocks() {
    
    std::cout << "\"blocks\":[";
    
    bool comma = false;
    for(auto it = blocks.begin(); it != blocks.end(); ++it) {
        Block* block = *it;
        
        if(comma) std::cout << ","; else comma = true;
        std::cout << "{";
        
        block->printBlock();
        
        std::cout << "}";
    }
    
    std::cout << "],";
}

void Block::printBlock() {
    
    std::cout << "\"id\":\"" << id << "\",";
    std::cout << "\"parameters\":[";
    
    bool comma = false;
    for(auto it = inputs.begin(); it != inputs.end(); ++it) {
        Parameter* parameter = it->second;
        
        if(comma) std::cout << ","; else comma = true;
        std::cout << "{";
        
        std::cout << "\"label\":\"" << it->first << "\",";
        std::cout << "\"type\":\"" << Parameter::typeToString(parameter->type) << "\",";
        std::cout << "\"value\":\"" << parameter->valueToString() << "\"";
        
        std::cout << "}";
    }
    
    std::cout << "],";
    std::cout << "\"outputs\":[";
    
    comma = false;
    for(auto it = outputs.begin(); it != outputs.end(); ++it) {
//        Unit* unit = it->second; TODO: need more information?
        
        if(comma) std::cout << ","; else comma = true;
        std::cout << "{";
        
        std::cout << "\"label\":\"" << it->first << "\"";
        
        std::cout << "}";
    }
    
    std::cout << "]";
    
}
