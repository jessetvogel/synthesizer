#include <iostream>
#include <portaudio.h>
#include <portmidi.h>

#include "controller.hpp"
#include "parser.hpp"
#include "basic.hpp"

#include "log.hpp"

#define MAIN_FILE "/Users/jessetvogel/Projects/synthesizer/synth/main.synth"

int main(int argc, char *argv[]) {
    // Initialize stuff (TODO: put this in some audio class or something, and check for errors)
    Pm_Initialize();
    Pa_Initialize();
    Basic::initialize();
    
    // Setup
    Controller* controller = new Controller();
    
    Log::output("Input devices");
    controller->listInputDevices();
    Log::output("\nOutput devices");
    controller->listOutputDevices();
    controller->setInputDevice(1);
    controller->setOutputDevice(1);
    Log::output("");
    
    Parser parser(controller, MAIN_FILE);
    if(parser.parse()) {

        controller->start();
        Log::output("Press enter to stop");
        getchar();
        controller->stop();

    }
    
    // Cleanup stuff
    delete controller;
    
    Basic::destruct();
    Pm_Terminate();
    Pa_Terminate();
    
    return 0;
}
