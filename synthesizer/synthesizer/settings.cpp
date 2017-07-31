#include <iostream>
#include <fstream>
#include <cmath>
#include "settings.hpp"
#include "commands.hpp"
#include "util.hpp"
#include "midistate.hpp"

#include "status.hpp"

Settings::Settings(std::string filepath) {
    // Set default values
    sampleRate = DEFAULT_SAMPLE_RATE;
    bufferSize = DEFAULT_BUFFER_SIZE;
    
    masterVolume = 1.0;
    masterVolumeCC = MIDI_MAIN_VOLUME;
    pitchWheelRange = DEFAULT_PITCH_WHEEL_RANGE;
    sustainPedalPolarity = DEFAULT_SUSTAIN_PEDAL_POLARITY;
    for(int i = 0;i < AMOUNT_OF_KEYS; i ++)
        frequencies[i] = 440.0 * std::pow(2.0, (double) (i - NOTE_A4) / 12.0);
    
    if(filepath.length() == 0) {
        Status::addWarning("No settings were provided");
        return;
    }
    
    // Load settings
    if(!load(filepath))
        Status::addError("Failed to load settings");
}

bool Settings::load(std::string filepath) {
    // Read file line by line, and parse them
    std::ifstream input(filepath);
    if(input.fail()) { Status::addError("Failed to open settings file"); return false; }
    
    int lineNumber = 1;
    std::string line;
    while(std::getline(input, line)) {
        if(!parseLine(line))
            return false;
        ++ lineNumber;
    }
    input.close();
    return true;
}

bool Settings::parseLine(std::string line) {
    // Neglect empty lines
    if(line.length() == 0) return true;
    
    std::cmatch cm;
    
    // Remove all surrounding whitespace and comments
    if(!std::regex_search(line.c_str(), cm, Commands::regexPreprocess)) { Status::addError("Unable to parse line"); return false; }
    std::string command = std::string(cm[1]);
    if(command.length() == 0) return true;
    const char* str = command.c_str();
    
    // Settings
    
    // settings_set <parameter> <value>
    if(std::regex_search(str, cm, Commands::regexSettingsSet)) {
        if(cm[1].compare("sample_rate") == 0) {
            sampleRate = stod(cm[2]);
            return true;
        }
        
        if(cm[1].compare("buffer_size") == 0) {
            bufferSize = stoi(cm[2]);
            return true;
        }
        
        return set(cm[1], cm[2]);
    }
    
    Status::addError("Command not recognised");
    return false;
}

bool Settings::set(std::string key, std::string value) {
    if(key.compare("master_volume_cc") == 0) {
        if(Util::isInteger(value)) {
            masterVolumeCC = stoi(value);
            return true;
        }
        return false;
    }
    
    if(key.compare("sustain_pedal_polarity") == 0) {
        if(value.compare("inverted") == 0) {
            sustainPedalPolarity = true;
            return true;
        }
        
        if(value.compare("normal") == 0) {
            sustainPedalPolarity = false;
            return true;
        }
        
        Status::addError("Invalid argument provided");
        return false;
    }
    
    if(key.compare("pitch_wheel_range") == 0) {
        if(Util::isNumber(value)) {
            pitchWheelRange = stod(value);
            return true;
        }
        
        Status::addError("Invalid argument provided");
        return false;
    }
    
    Status::addError("Invalid setting provided");
    return false;
}
