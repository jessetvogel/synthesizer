#include <iostream>
#include <portaudio.h>
#include <portmidi.h>

#include "controller.hpp"
#include "parser.hpp"
#include "settings.hpp"

#include "sample.hpp"
#include "curve.hpp"
#include "function.hpp"

#include "status.hpp"

void initialize();
void destruct();

int main(int argc, char *argv[]) {
    // Load settings
    Settings settings;
    
    // Initialize
    initialize();
    
    // Create a new controller object
    Controller controller(&settings);
    
    // Load settings
    Parser parser(&controller);
    parser.setDirectory(settings.rootDirectory);
    
    Status::addInfo("Program started");
    Status::print(&controller);
    
    // Wait for input
    std::string line;
    while(std::getline(std::cin, line)) {
        // Check for exit command
        if(line.compare("exit") == 0) break;
        
        // Try to parse the given line
        bool success = parser.parseLine(line);
        
        if(!success && Status::noErrors())
            Status::addError("Unknown error");
        
        Status::print(&controller);
    }
    
    controller.stop();
    
    // Clean up
    destruct();
    
    Status::addInfo("Program ended");
    Status::print(&controller);
    
    return 0;
}

void initialize() {
    Pm_Initialize();
    Pa_Initialize();
    Sample::initialize();
    Curve::initialize();
    Function::initialize();
}

void destruct() {
    Function::destruct();
    Curve::destruct();
    Sample::destruct();
    Pa_Terminate();
    Pm_Terminate();
}
