#include <iostream>
#include <portaudio.h>
#include <portmidi.h>

#include "controller.hpp"
#include "parser.hpp"
#include "settings.hpp"
#include "sample.hpp"
#include "commands.hpp"

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
    
                        // Show input/output devices
                        Log::output("Input devices");
                        controller.listInputDevices();
                        Log::output("\nOutput devices");
                        controller.listOutputDevices();
                        Log::output("");
    
    // Load settings
    Parser parser(&controller);
    parser.parseFile(MAIN_DIRECTORY DIRECTORY_SEPARATOR SETTINGS_FILE);
    
    // Wait for input
    parser.setDirectory(MAIN_DIRECTORY);
    std::string line;
    while(std::getline(std::cin, line)) {
        // Check for exit command
        if(line.compare("exit") == 0) break;
        
        // Try to parse the given line
        if(!parser.parseLine(line)) {
            std::cout << "Was not able to parse '" << line << "'" << std::endl;
        }
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
