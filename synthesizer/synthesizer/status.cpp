#include <iostream>

#include "status.hpp"
#include "controller.hpp"
#include "input.hpp"
#include "output.hpp"

Status::Status(Controller* controller) {
    // Save pointer to controller
    this->controller = controller;
}

bool Status::print(std::string info) {
    if(info.compare("midi_devices") == 0) {
        printMidiDevices();
        return true;
    }
    
    if(info.compare("output_devices") == 0) {
        printOutputDevices();
        return true;
    }
    
    return false;
}

void Status::printMidiDevices() {
    
    std::cout << "{";
    
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
    
    std::cout << "]";
    
    std::cout << "}" << std::endl;
}

void Status::printOutputDevices() {
    
    std::cout << "{";
    
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
    
    std::cout << "]";
    
    std::cout << "}" << std::endl;
}
