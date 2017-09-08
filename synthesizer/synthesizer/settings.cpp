#include <fstream>
#include <cmath>
#include "settings.hpp"
#include "commands.hpp"
#include "util.hpp"
#include "keystate.hpp"

#include "status.hpp"

Settings::Settings(std::string dir) : directory(dir) {
    // Set default values (note the end of this function)
    newSampleRate = DEFAULT_SAMPLE_RATE;
    newBufferSize = DEFAULT_BUFFER_SIZE;
    newVoices = DEFAULT_VOICES;
    
    masterVolume = 1.0;
    masterVolumeCC = MIDI_MAIN_VOLUME;
    pitchWheelRange = DEFAULT_PITCH_WHEEL_RANGE;
    sustainPedalPolarity = DEFAULT_SUSTAIN_PEDAL_POLARITY;
    for(int i = 0;i < AMOUNT_OF_KEYS; i ++)
        frequencies[i] = 440.0 * std::pow(2.0, (double) (i - NOTE_A4) / 12.0);
    rootDirectory = "";
    
    if(directory.length() == 0) {
        Status::addWarning("No settings directory was provided");
    }
    else {
        // Load settings
        if(!load())
        Status::addError("Failed to load settings");
    }
    
    // Set constants to the ones loaded just now
    sampleRate = newSampleRate;
    bufferSize = newBufferSize;
    voices = newVoices;
}

bool Settings::load() {
    // Read file line by line, and parse them
    std::ifstream input(directory + DIRECTORY_SEPARATOR + "settings");
    if(input.fail()) { Status::addError("Failed to open settings file to load data"); return false; }
    
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

bool Settings::store() {
    // If no filepath given, just don't store.
    if(directory.length() == 0) return true;
    
    // Write file line by line
    std::ofstream output(directory + DIRECTORY_SEPARATOR + "settings");
    if(output.fail()) { Status::addError("Failed to open settings file to store data"); return false; }
    
    // Constants
    output << "settings_set sample_rate " << newSampleRate << std::endl;
    output << "settings_set buffer_size " << newBufferSize << std::endl;
    output << "settings_set voices " << newVoices << std::endl;
    
    // Changeable
    output << "settings_set master_volume_cc " << masterVolumeCC << std::endl;
    output << "settings_set sustain_pedal_polarity " << (sustainPedalPolarity ? "inverted" : "normal") << std::endl;
    output << "settings_set pitch_wheel_range " << pitchWheelRange << std::endl;
    
    output.close();
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
    if(std::regex_search(str, cm, Commands::regexSettingsSet))
        return set(cm[1], cm[2]);
    
    Status::addError("Command not recognised");
    return false;
}

bool Settings::set(std::string key, std::string value) {
    // Constants (we use 'new...' because we shouldn't use these, only save them so that we can use them next time)
    if(key.compare("sample_rate") == 0) {
        if(Util::isNumber(value)) {
            newSampleRate = stod(value);
            return true;
        }
        Status::addError("Invalid argument provided");
        return false;
    }
    
    if(key.compare("buffer_size") == 0) {
        if(Util::isInteger(value)) {
            newBufferSize = stod(value);
            return true;
        }
        Status::addError("Invalid argument provided");
        return false;
    }
    
    if(key.compare("voices") == 0) {
        if(Util::isNumber(value)) {
            newVoices = stod(value);
            return true;
        }
        Status::addError("Invalid argument provided");
        return false;
    }
    
    // Changeable
    if(key.compare("master_volume_cc") == 0) {
        if(Util::isInteger(value)) {
            masterVolumeCC = stoi(value);
            return true;
        }
        Status::addError("Invalid argument provided");
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
    
    if(key.compare("root_directory") == 0) {
        rootDirectory = value;
        return true;
    }
    
    Status::addError("Invalid setting provided");
    return false;
}
