#include <iostream>
#include <portaudio.h>
#include <portmidi.h>

#include "controller.hpp"
#include "parser.hpp"
#include "settings.hpp"
#include "sample.hpp"
#include "curve.hpp"
#include "function.hpp"
#include "commands.hpp"
#include "status.hpp"

#include "error.hpp"

void initialize();
void destruct();

int main(int argc, char *argv[]) {
    // Initialize stuff (TODO: put this in some audio class or something, and check for errors)
    initialize();
    
    // Load settings
    Settings settings;
    
    // Create a new controller object
    Controller controller(&settings);
    
    // Load settings
    Parser parser(&controller);
    parser.setDirectory(settings.rootDirectory);
    
    // Wait for input
    std::string line;
    while(std::getline(std::cin, line)) {
        // Check for exit command
        if(line.compare("exit") == 0) break;
        
        std::cout << "{";
        
        // Try to parse the given line
        bool success = parser.parseLine(line);
        
        if(!success && Error::noErrors())
            Error::addError(Error::UNKNOWN);
        Error::printErrors();
        
        std::cout << "}" << std::endl;
        std::cout.flush();
    }
    
    controller.stop();
    
    // Clean up
    destruct();
    
    return 0;
}

void initialize() {
    Pm_Initialize();
    Pa_Initialize();
    Sample::initialize();
    Curve::initialize();
    Function::initialize();
    Status::start();
}

void destruct() {
    Function::destruct();
    Curve::destruct();
    Sample::destruct();
    Pa_Terminate();
    Pm_Terminate();
    Status::stop();
}
