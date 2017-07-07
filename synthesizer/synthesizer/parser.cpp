#include <iostream>
#include <fstream>
#include <regex>

#include "parser.hpp"
#include "commands.hpp"
#include "controller.hpp"
#include "audiodevices.hpp"
#include "mididevices.hpp"
#include "units.hpp"
#include "unit.hpp"
#include "blocks.hpp"
#include "options.hpp"

#include "status.hpp"

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
        Status::addError("Failed to open file");
        return false;
    }
    
    int lineNumber = 1;
    std::string line;
    while(std::getline(input, line)) {
        if(!parseLine(line)) {
            Status::addError(line);
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
        Status::addError("Unable to parse line");
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
    
    // play <state>
    if(std::regex_search(str, cm, Commands::regexPlay))                     return controller->play(cm[1]);
    // reset
    if(std::regex_search(str, cm, Commands::regexReset))                    return controller->reset();
    
    // Status
    
    // status <info>
    if(std::regex_search(str, cm, Commands::regexStatus))                   return Status::addExtra(cm[1]);
    
    // Synths
    
    // include <filename>
    if(std::regex_search(str, cm, Commands::regexInclude)) {
        Parser parser(controller);
        parser.parseFile(directory + DIRECTORY_SEPARATOR + std::string(cm[1]));
        return true;
    }
    
    // Units
    
    // unit_create <unit_type> <label> <args>
    if(std::regex_search(str, cm, Commands::regexUnitCreate))               return controller->getUnits()->create(cm[1], cm[2], cm[3]);
    // unit_rename <unit> <label>
    if(std::regex_search(str, cm, Commands::regexUnitRename))               return controller->getUnits()->rename(cm[1], cm[2]);
    // unit_delete <unit>
    if(std::regex_search(str, cm, Commands::regexUnitDelete))               return controller->getUnits()->destroy(cm[1]);
    // unit_hide <unit>
    if(std::regex_search(str, cm, Commands::regexUnitHide))                 return controller->getUnits()->hide(cm[1]);
    // unit_set <unit> <parameter> <value>
    if(std::regex_search(str, cm, Commands::regexUnitSet))                  return controller->getUnits()->set(cm[1], cm[2], cm[3]);
    
    // Blocks
    
    // block_create <id>
    if(std::regex_search(str, cm, Commands::regexBlockCreate))              return controller->getBlocks()->create(cm[1]);
    // block_delete <id>
    if(std::regex_search(str, cm, Commands::regexBlockDelete))              return controller->getBlocks()->destroy(cm[1]);
    // block_add_input <id> <label> <unit> <parameter>
    if(std::regex_search(str, cm, Commands::regexBlockAddInput))            return controller->getBlocks()->addInput(cm[1], cm[2], cm[3], cm[4]);
    // block_add_output <id> <label> <unit>
    if(std::regex_search(str, cm, Commands::regexBlockAddOutput))           return controller->getBlocks()->addOutput(cm[1], cm[2], cm[3]);
    // block_set <id> <parameter> <value>
    if(std::regex_search(str, cm, Commands::regexBlockSet))                 return controller->getBlocks()->set(cm[1], cm[2], cm[3]);

    // Unknown command
    Status::addError("Command not recognised");
    return false;
}
