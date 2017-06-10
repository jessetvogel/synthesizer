#include <iostream>

#include "status.hpp"
#include "controller.hpp"
#include "input.hpp"
#include "output.hpp"

Status::Status(Controller* controller) {
    // Save pointer to controller
    this->controller = controller;
}

void Status::print() {
    // Print according as described in doc/status.json
    std::cout << "{";
    
    printInputDevices(); std::cout << ",";
    printOutputDevices();
    
    std::cout << "}" << std::endl;
}

void Status::printInputDevices() {
    std::cout << "\"inputDevices\": [";
    
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
    
    std::cout << "]";
}
