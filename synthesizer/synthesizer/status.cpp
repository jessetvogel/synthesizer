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

std::vector<Message> Status::info;
std::vector<Message> Status::warning;
std::vector<Message> Status::error;

std::vector<std::string> Status::extra;

void Status::addInfo(std::string message) {
    Message i;
    i.message = message;
    info.push_back(i);
}

void Status::addWarning(std::string message) {
    Message w;
    w.message = message;
    warning.push_back(w);
}

void Status::addError(std::string message) {
    Message e;
    e.message = message;
    error.push_back(e);
}

bool Status::addExtra(std::string tokens) {
    std::istringstream iss(tokens);
    std::string token;
    while(std::getline(iss, token, ' '))
        extra.push_back(token);
    return true;
}

bool Status::print(Controller* controller) {
    
    std::cout << "{";
    
    bool comma = false;
    
    // Print messages
    if(!info.empty()) {
        if(comma) std::cout << ","; else comma = true;
        printInfo();
    }
    
    if(!warning.empty()) {
        if(comma) std::cout << ","; else comma = true;
        printWarning();
    }
    
    if(!error.empty()) {
        if(comma) std::cout << ","; else comma = true;
        printError();
    }
    
    for(auto it = extra.begin();it != extra.end(); ++it) {
        if(comma) std::cout << ","; else comma = true;
        printExtra(controller, *it);
    }
    
    std::cout << "}" << std::endl;
    std::cout.flush();
    
    info.clear();
    warning.clear();
    error.clear();
    extra.clear();
    
    return true;
}

void Status::printInfo() {
    
    std::cout << "\"info\":[";
    
    bool comma = false;
    for(auto it = info.begin();it != info.end(); ++it) {
        if(comma) std::cout << ","; else comma = true;
        
        std::cout << "{";
        
        std::cout << "\"message\":\"" << (*it).message << "\"";
        
        std::cout << "}";
    }
    
    std::cout << "]";
}

void Status::printWarning() {
    
    std::cout << "\"warning\":[";
    
    bool comma = false;
    for(auto it = warning.begin();it != warning.end(); ++it) {
        if(comma) std::cout << ","; else comma = true;
        
        std::cout << "{";
        
        std::cout << "\"message\":\"" << (*it).message << "\"";
        
        std::cout << "}";
    }
    
    std::cout << "]";
}

void Status::printError() {
    
    std::cout << "\"error\":[";
    
    bool comma = false;
    for(auto it = error.begin();it != error.end(); ++it) {
        if(comma) std::cout << ","; else comma = true;
        
        std::cout << "{";
        
        std::cout << "\"message\":\"" << (*it).message << "\"";
        
        std::cout << "}";
    }
    
    std::cout << "]";
}

void Status::printExtra(Controller* controller, std::string extra) {
    if(extra.compare("midi_devices") == 0) return controller->getMIDIDevices()->printMIDIDevices();
    if(extra.compare("input_devices") == 0) return controller->getAudioDevices()->printInputDevices();
    if(extra.compare("output_devices") == 0) return controller->getAudioDevices()->printOutputDevices();
    if(extra.compare("instruments") == 0) return controller->getInstruments()->printInstruments();
    if(extra.compare("units") == 0) return controller->getUnits()->printUnits();
    if(extra.compare("blocks") == 0) return controller->getBlocks()->printBlocks();
}

// ----------------------------------------------------------------

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
            
            std::cout << "\"id\":" << i << ",";
            std::cout << "\"name\":\"" << MIDIDevices::deviceName(i) << "\",";
            std::cout << "\"active\":" << (active ? "true" : "false");
            
            std::cout << "}";
        }
    }
    
    std::cout << "]";
    
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
    
    std::cout << "]";
    
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
    
    std::cout << "]";
    
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
    
    std::cout << "]";
    
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
    
    std::cout << "]";
    
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
        std::cout << "\"value\":\"" << parameter->strValue << "\"";
        
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
    
    std::cout << "]";
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
        std::cout << "\"value\":\"" << parameter->strValue << "\"";
        
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
