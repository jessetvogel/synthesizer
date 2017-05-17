#include <iostream>
#include <portaudio.h>
#include <portmidi.h>
#include "controller.hpp"
#include "ADSR.hpp"
#include "log.hpp"

#include <cmath>

int main(int argc, char *argv[]) {
    // Initialize stuff (TODO: put this in some audio class or something, and check for errors)
    Pm_Initialize();
    Pa_Initialize();
    Sample::initialize();
    Envelope::initialize();
    
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
    instrument->setSample(Sample::triangle);
    ADSR* adsr = new ADSR();
    adsr->attack = 0.01;
    adsr->decay = 0.5;
    adsr->sustain = 0.5f;
    adsr->release = 0.05;

    instrument->setEnvelope(adsr);
    
    controller->addInstrument(instrument);
    
    controller->start();
    
    Log::output("Press enter to stop");
    getchar();
    
    controller->stop();
    
    // Cleanup stuff
    delete controller;
    
    // TODO: cleanup samples and envelopes
    
    Pm_Terminate();
    Pa_Terminate();
    
    return 0;
}
