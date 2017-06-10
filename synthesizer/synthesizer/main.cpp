#include <iostream>
#include <portaudio.h>
#include <portmidi.h>

#include "controller.hpp"
#include "parser.hpp"
#include "settings.hpp"
#include "sample.hpp"
#include "commands.hpp"
#include "error.hpp"

#include "log.hpp"

#define MAIN_DIRECTORY "/Users/Jesse/Projects/synthesizer/files"
#define SETTINGS_FILE "settings.txt"

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
    parser.parseFile(MAIN_DIRECTORY DIRECTORY_SEPARATOR SETTINGS_FILE);
    
    // Wait for input
    parser.setDirectory(MAIN_DIRECTORY);
    std::string line;
    std::cout << "> "; std::cout.flush();
    while(std::getline(std::cin, line)) {
        // Check for exit command
        if(line.compare("exit") == 0) break;
        
        // Try to parse the given line
        if(!parser.parseLine(line))
            Error::printLastError();
        
        std::cout << "> "; std::cout.flush();
    }
    
    controller.stop();
    
    std::cout << "Waiting 100msec for stream to end" << std::endl;
    Pa_Sleep(100);
    
    // Clean up
    destruct();
    
    return 0;
}

void initialize() {
    Pm_Initialize();
    Pa_Initialize();
    Sample::initialize();
}

void destruct() {
    Sample::destruct();
    Pa_Terminate();
    Pm_Terminate();
}
