#include <iostream>
#include <sstream>

#include "status.hpp"
#include "controller.hpp"
#include "input.hpp"
#include "output.hpp"

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
            std::cout << "\"name\": \"" << Input::deviceName(i) << "\"";
            
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
