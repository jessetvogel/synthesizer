#include <iostream>
#include <fstream>
#include "settings.hpp"
#include "error.hpp"
#include "commands.hpp"

Settings::Settings() {
    // Set default values
    sampleRate = DEFAULT_SAMPLE_RATE;
    bufferSize = DEFAULT_BUFFER_SIZE;
    
    rootDirectory = DEFAULT_ROOT_DIRECTORY;
    
    // Load settings
    if(!load())
        Error::addError("Error in loading settings");
}

bool Settings::load() {
    // Read file line by line, and parse them
    std::ifstream input(SETTINGS_PATH);
    if(input.fail()) {
        Error::addError(Error::CANNOT_OPEN_FILE);
        return false;
    }
    
    int lineNumber = 1;
    std::string line;
    while(std::getline(input, line)) {
        if(!parseLine(line)) {
            std::cout << "Error in line: " << line << std::endl;
            return false;
        }
        
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
    if(!std::regex_search(line.c_str(), cm, Commands::regexPreprocess)) {
        Error::addError(Error::COMMAND_NOT_RECOGNISED);
        return false;
    }
    std::string command = std::string(cm[1]);
    if(command.length() == 0) return true;
    const char* str = command.c_str();
    
    // Settings
    
    // settings_set_sample_rate <value>
    if(std::regex_search(str, cm, Commands::regexSetSampleRate)) {
        sampleRate = stod(cm[1]);
        return true;
    }
    
    // settings_set_buffer_size <value>
    if(std::regex_search(str, cm, Commands::regexSetBufferSize)) {
        bufferSize = stoi(cm[1]);
        return true;
    }
    
    // settings_set_root_directory <directory>
    if(std::regex_search(str, cm, Commands::regexSetRootDirectory)) {
        rootDirectory = cm[1];
        return true;
    }
    
    Error::addError(Error::COMMAND_NOT_RECOGNISED);
    return false;
}
