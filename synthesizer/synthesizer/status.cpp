#include <iostream>
#include <sstream>

#include "status.hpp"
#include "controller.hpp"
#include "input.hpp"
#include "output.hpp"
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
            printMidiDevices();
            continue;
        }
        
        if(token.compare("output_devices") == 0) {
            printOutputDevices();
            continue;
        }
        
        if(token.compare("units") == 0) {
            printUnits();
            continue;
        }
        
        if(!error) {
            Error::addError(Error::INVALID_ARGUMENT);
            error = true;
        }
    }
    
    return true;
}

void Status::printMidiDevices() {
    
    std::cout << "\"midiDevices\": [";
    
    int amountOfDevices = Input::amountOfDevices();
    bool comma = false;
    for(int i = 0;i < amountOfDevices;i ++) {
        if(Input::isInput(i)) {
            if(comma) std::cout << ","; else comma = true; // Make sure devices get separated by commas
            std::cout << "{";
            
            std::cout << "\"id\": " << i << ", ";
            std::cout << "\"name\": \"" << Input::deviceName(i) << "\", ";
            std::cout << "\"active\": " << (controller->inputActive(i) ? "true" : "false");
            
            std::cout << "}";
        }
    }
    
    std::cout << "],";
    
}

void Status::printOutputDevices() {
    
    std::cout << "\"outputDevices\": [";
    
    int amountOfDevices = Output::amountOfDevices();
    bool comma = false;
    for(int i = 0;i < amountOfDevices;i ++) {
        if(Output::isOutput(i)) {
            if(comma) std::cout << ","; else comma = true; // Make sure devices get separated by commas
            std::cout << "{";
            
            std::cout << "\"id\": " << i << ", ";
            std::cout << "\"name\": \"" << Output::deviceName(i) << "\"";
            
            std::cout << "}";
        }
    }
    
    std::cout << "],";
    
}

void Status::printUnits() {
    
    std::cout << "\"units\": [";
    
    bool comma = false;
    for(auto it = controller->units.begin(); it != controller->units.end(); ++it) {
        Unit* unit = it->second;
        
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
