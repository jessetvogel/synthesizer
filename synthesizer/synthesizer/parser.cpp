#include <iostream>
#include <fstream>
#include <regex>

#include "parser.hpp"
#include "commands.hpp"
#include "controller.hpp"
#include "audiodevices.hpp"
#include "mididevices.hpp"
#include "nodes.hpp"
#include "node.hpp"
#include "monitor.hpp"
#include "settings.hpp"

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
    
    // Settings
    
    // settings_set <parameter> <value>
    if(std::regex_search(str, cm, Commands::regexSettingsSet))              return controller->getSettings()->set(cm[1], cm[2]);
    
    // Controller
    
    // play <state>
    if(std::regex_search(str, cm, Commands::regexPlay))                     return controller->play(cm[1]);
    // reset
    if(std::regex_search(str, cm, Commands::regexClear))                    return controller->clear();
    
    // Status
    
    // status <info>
    if(std::regex_search(str, cm, Commands::regexStatus))                   return Status::addExtra(cm[1]);
    
    // Monitor
    
    // monitor <node output>
    if(std::regex_search(str, cm, Commands::regexMonitor))                  return controller->getMonitor()->monitor(cm[1]);
    
    // Synths
    
    // include <filename>
    if(std::regex_search(str, cm, Commands::regexInclude)) {
        Parser parser(controller);
        parser.parseFile(directory + DIRECTORY_SEPARATOR + std::string(cm[1]));
        return true;
    }
    
    // Nodes
    
    // node_create <node type> <id> [<options>]?
    if(std::regex_search(str, cm, Commands::regexNodeCreate))               return controller->getNodes()->create(cm[1], cm[2], cm[3]);
    // node_rename <node id> <new node id>
    if(std::regex_search(str, cm, Commands::regexNodeRename))               return controller->getNodes()->rename(cm[1], cm[2]);
    // node_delete <node id>
    if(std::regex_search(str, cm, Commands::regexNodeDelete))               return controller->getNodes()->destroy(cm[1]);
    // node_set <node input> <value>
    if(std::regex_search(str, cm, Commands::regexNodeSet))                  return controller->getNodes()->set(cm[1], cm[2]);
    // node_hide <node id>
    if(std::regex_search(str, cm, Commands::regexNodeHide))                 return controller->getNodes()->hide(cm[1]);
    // node_info <node id>
    if(std::regex_search(str, cm, Commands::regexNodeInfo))                 return controller->getNodes()->info(cm[1]);
    
    // Custom nodes
    
    // node_add_input <node id> <label> <node input>
    if(std::regex_search(str, cm, Commands::regexNodeAddInput))             return controller->getNodes()->addInput(cm[1], cm[2], cm[3]);
    // node_add_output <node id> <label> <node output>
    if(std::regex_search(str, cm, Commands::regexNodeAddOutput))            return controller->getNodes()->addOutput(cm[1], cm[2], cm[3]);
    // node_attach <node id> <node id>
    if(std::regex_search(str, cm, Commands::regexNodeAttach))               return controller->getNodes()->attach(cm[1], cm[2]);
    
    // Unknown command
    Status::addError("Command not recognised");
    return false;
}
