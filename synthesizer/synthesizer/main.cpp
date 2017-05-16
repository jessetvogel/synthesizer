#include <iostream>
#include <portaudio.h>
#include <portmidi.h>
#include "controller.hpp"
#include "log.hpp"

#include <cmath>

int main(int argc, char *argv[]) {
    // Initialize stuff (TODO: put this in some audio class or something, and check for errors)
    Pm_Initialize();
    Pa_Initialize();
    
    // Create samples
    float dataSine[512];
    float dataSquare[512];
    for(int i = 0;i < 512;i ++) {
        dataSine[i] = sin((double) i / 512 * 2.0 * M_PI);
        dataSquare[i] = i < 256 ? -1.0 : 1.0;
    }
    Sample* sine = new Sample(512, dataSine);
    Sample* square = new Sample(512, dataSquare);
    Sample* sample = new Sample(sine, square, 0.5);
    
    // Setup
    Controller* controller = new Controller();
    
    Log::output("Input devices");
    controller->listInputDevices();
    Log::output("\nOutput devices");
    controller->listOutputDevices();
    controller->setInputDevice(1);
    controller->setOutputDevice(1);
    Log::output("");
    
    Instrument* instrument = new Instrument(controller);
    instrument->setSample(sine);
    controller->addInstrument(instrument);
    
    controller->start();
    
    Log::output("Press enter to stop");
    getchar();
    
    controller->stop();
    
    // Cleanup stuff
    delete controller;
    
    delete sine;
    delete square;
    delete sample;
    
    Pm_Terminate();
    Pa_Terminate();
    
    return 0;
}
