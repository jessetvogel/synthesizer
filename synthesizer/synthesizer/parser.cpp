#include <iostream>
#include <fstream>
#include <regex>

#include "parser.hpp"
#include "commands.hpp"
#include "controller.hpp"
#include "audiodevices.hpp"
#include "mididevices.hpp"
#include "instruments.hpp"
#include "instrument.hpp"
#include "units.hpp"
#include "unit.hpp"
#include "options.hpp"
#include "status.hpp"

#include "error.hpp"

Parser::Parser(Controller* controller) {
    // Store pointer to controller
    this->controller = controller;
}

bool Parser::parseFile(std::string filepath) {
    // Store filepath and determine the directory
    this->filepath = filepath;
    directory = filepath.substr(0, filepath.find_last_of(DIRECTORY_SEPARATOR));
    
    // Read file line by line, and parse them
    std::ifstream input(filepath);
    if(input.fail()) {
        Error::addError(Error::CANNOT_OPEN_FILE);
        return false;
    }
    
    int lineNumber = 1;
    std::string line;
    while(std::getline(input, line)) {
        if(!parseLine(line)) {
            Error::addError("Error in file"); // TODO
            std::cout << "Error in line: " << line << std::endl;
            return false;
        }
        
        ++ lineNumber;
    }
    input.close();
    return true;
}

bool Parser::parseLine(std::string line) {
    // Neglect empty lines
    if(line.length() == 0) return true;
    
    std::cmatch cm;

    // Remove all surrounding whitespace and comments
    if(!std::regex_search(line.c_str(), cm, Commands::regexPreprocess)) {
        Error::addError(Error::COMMAND_NOT_RECOGNISED);
        return false;
    }
    std::string command = std::string(cm[1]);
    if(command.length() == 0) return true;
    const char* str = command.c_str();
    
    // MIDI

    // midi_add_device <input_device>
    if(std::regex_search(str, cm, Commands::regexMidiAddDevice))            return controller->getMIDIDevices()->add(stoi(cm[1]));
    // midi_remove_device <input_device>
    if(std::regex_search(str, cm, Commands::regexMidiRemoveDevice))         return controller->getMIDIDevices()->remove(stoi(cm[1]));
    
    // Audio
    
    // audio_set_input_device <input_device>
    if(std::regex_search(str, cm, Commands::regexAudioSetInputDevice))      return controller->getAudioDevices()->setInputDeviceId(stoi(cm[1]));
    // audio_set_output_device <input_device>
    if(std::regex_search(str, cm, Commands::regexAudioSetOutputDevice))     return controller->getAudioDevices()->setOutputDeviceId(stoi(cm[1]));
    
    // Options
    
    // options_set_sustain_pedal_polarity <normal|inverted>
    if(std::regex_search(str, cm, Commands::regexSetSustainPedalPolarity))  return controller->getOptions()->setSustainPedalPolarity(cm[1]);
    // options_set_pitch_wheel_range <semitones>
    if(std::regex_search(str, cm, Commands::regexSetPitchWheelRange))       return controller->getOptions()->setPitchWheelRange(stod(cm[1]));
    
    // Controller
    
    // start
    if(std::regex_search(str, cm, Commands::regexStart))                    return controller->start();
    // stop
    if(std::regex_search(str, cm, Commands::regexStop))                     return controller->stop();
    // reset
    if(std::regex_search(str, cm, Commands::regexReset)) /* TODO!! return controller->reset(); */ return true;
    
    // Status
    
    // status <info>
    if(std::regex_search(str, cm, Commands::regexStatus)) {
        Status status(controller);
        return status.print(cm[1]);
    }
    
    // Synths
    
    // include <filename>
    if(std::regex_search(str, cm, Commands::regexInclude)) {
        Parser parser(controller);
        parser.parseFile(directory + DIRECTORY_SEPARATOR + std::string(cm[1]));
        return true;
    }
        
    // Instruments
    
    // instrument_create <label>
    if(std::regex_search(str, cm, Commands::regexInstrumentCreate))         return controller->getInstruments()->create(cm[1]);    
    // instrument_delete <instrument>
    if(std::regex_search(str, cm, Commands::regexInstrumentDelete))         return controller->getInstruments()->remove(cm[1]);
    // instrument_set_active <instrument> <true|false>
    if(std::regex_search(str, cm, Commands::regexInstrumentSetActive))      return controller->getInstruments()->setActive(cm[1], cm[2].compare("true") == 0);
    // instrument_set_output <instrument> <unit>
    if(std::regex_search(str, cm, Commands::regexInstrumentSetOutput))      return controller->getInstruments()->setOutput(cm[1], cm[2]);
    // instrument_set_key_output <instrument> <key_unit>
    if(std::regex_search(str, cm, Commands::regexInstrumentSetKeyOutput))   return controller->getInstruments()->setKeyOutput(cm[1], cm[2]);    
    // instrument_set_key_release_time <instrument> <seconds>
    if(std::regex_search(str, cm, Commands::regexInstrumentSetKeyReleaseTime)) return controller->getInstruments()->setKeyReleaseTime(cm[1], stod(cm[2]));

    // Units
    
    // unit_create <unit_type> <label> <args>
    if(std::regex_search(str, cm, Commands::regexUnitCreate))               return controller->getUnits()->create(cm[1], cm[2], cm[3]);
    // unit_delete <unit>
    if(std::regex_search(str, cm, Commands::regexUnitDelete))               return controller->getUnits()->remove(cm[1]);
    // unit_rename <unit> <label>
    if(std::regex_search(str, cm, Commands::regexUnitRename))               return controller->getUnits()->rename(cm[1], cm[2]);
    // unit_set_value <unit> <parameter> <value>
    if(std::regex_search(str, cm, Commands::regexUnitSetValue))             return controller->getUnits()->setValue(cm[1], cm[2], cm[3]);
    
    Error::addError(Error::COMMAND_NOT_RECOGNISED);
    return false;
}
