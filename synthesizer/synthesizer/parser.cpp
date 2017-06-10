#include <fstream>
#include <regex>

#include "parser.hpp"
#include "commands.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "unit.hpp"
#include "settings.hpp"
#include "status.hpp"

#include "error.hpp"
#include "log.hpp"

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
        char message[128 + filepath.length()];
        sprintf(message, "Was not able to open file %s", filepath.c_str());
        Log::error(message);
        return false;
    }
    
    int lineNumber = 1;
    std::string line;
    while(std::getline(input, line)) {
        if(!parseLine(line)) {
            char message[128 + filepath.length()];
            sprintf(message, "An error occured in line %d of file %s", lineNumber, filepath.c_str());
            Log::error(message);
            Log::error(line.c_str());
            Error::printLastError();
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
        Error::lastError = Error::COMMAND_NOT_RECOGNISED;
        return false;
    }
    std::string command = std::string(cm[1]);
    if(command.length() == 0) return true;
    const char* str = command.c_str();
    
    // MIDI

    // midi_add_input_device <input_device>
    if(std::regex_search(str, cm, Commands::regexMidiAddInputDevice)) {
        return controller->addInputDevice(stoi(cm[1]));
    }
    
    // midi_remove_input_device <input_device>
    if(std::regex_search(str, cm, Commands::regexMidiRemoveInputDevice)) {
        return controller->removeInputDevice(stoi(cm[1]));
    }
    
    // Settings
    
    // settings_set_sample_rate <value>
    if(std::regex_search(str, cm, Commands::regexSetSampleRate)) {
        controller->getSettings()->sampleRate = stod(cm[1]);
        return true;
    }
    
    // settings_set_buffer_size <value>
    if(std::regex_search(str, cm, Commands::regexSetBufferSize)) {
        controller->getSettings()->bufferSize = stoi(cm[1]);
        return true;
    }
    
    // settings_set_sustain_pedal_polarity <normal|inverted>
    if(std::regex_search(str, cm, Commands::regexSetSustainPedalPolarity)) {
        controller->getSettings()->sustainPedalPolarity = (cm[0].compare("inverted") == 0);
        return true;
    }
    
    // settings_set_pitch_wheel_range <semitones>
    if(std::regex_search(str, cm, Commands::regexSetPitchWheelRange)) {
        controller->getSettings()->pitchWheelRange = stod(cm[1]);
        return true;
    }
    
    // Controller
    
    // start
    if(std::regex_search(str, cm, Commands::regexStart)) {
        int outputDevice = (std::string(cm[1]).length() == 0) ? -1 : stoi(cm[1]);
        return controller->start(outputDevice);
    }
    
    // stop
    if(std::regex_search(str, cm, Commands::regexStop)) {
        return controller->stop();
    }
    
    // reset
    if(std::regex_search(str, cm, Commands::regexReset)) {
        return controller->reset();
    }
    
    // Status
    
    // status
    if(std::regex_search(str, cm, Commands::regexStatus)) {
        Status status(controller);
        status.print();
        return true;
    }
    
    // Synths
    
    // include <filename>
    if(std::regex_search(str, cm, Commands::regexInclude)) {
        Parser parser(controller);
        parser.parseFile(directory + DIRECTORY_SEPARATOR + std::string(cm[1]));
        return true;
    }
    
    // instrument_create <label>
    if(std::regex_search(str, cm, Commands::regexInstrumentCreate)) {
        Instrument* instrument = new Instrument(controller);
        if(!(controller->addInstrument(instrument, cm[1]))) {
            delete instrument;
            return false;
        };
        return true;
    }
    
    // instrument_delete <instrument>
    if(std::regex_search(str, cm, Commands::regexInstrumentDelete)) {
        return controller->deleteInstrument(cm[1]);
    }
    
    // instrument_set_output <instrument> <unit>
    if(std::regex_search(str, cm, Commands::regexInstrumentSetOutput)) {
        Instrument* instrument = controller->getInstrument(cm[1]);
        if(instrument == NULL) {
            Error::lastError = Error::INSTRUMENT_NOT_FOUND;
            return false;
        }
        Unit* unit = controller->getUnit(cm[2]);
        if(unit == NULL) {
            Error::lastError = Error::UNIT_NOT_FOUND;
            return false;
        }
        return instrument->setOutput(unit);
    }
    
    // instrument_set_key_output <instrument> <key_unit>
    if(std::regex_search(str, cm, Commands::regexInstrumentSetKeyOutput)) {
        Instrument* instrument = controller->getInstrument(cm[1]);
        if(instrument == NULL) {
            Error::lastError = Error::INSTRUMENT_NOT_FOUND;
            return false;
        }
        Unit* unit = controller->getUnit(cm[2]);
        if(unit == NULL) {
            Error::lastError = Error::UNIT_NOT_FOUND;
            return false;
        }
        return instrument->setKeyOutput(unit);
    }
    
    // instrument_set_key_release_time <instrument> <seconds>
    if(std::regex_search(str, cm, Commands::regexInstrumentSetKeyReleaseTime)) {
        Instrument* instrument = controller->getInstrument(cm[1]);
        if(instrument == NULL) {
            Error::lastError = Error::INSTRUMENT_NOT_FOUND;
            return false;
        }

        instrument->setKeyReleaseTime(stod(cm[2]));
        return true;
    }
    
    // unit_create <unit_type> <label> <arg1> <arg2>
    if(std::regex_search(str, cm, Commands::regexUnitCreate)) {
        Unit* unit = Unit::create(controller, cm[1], false, cm[3], cm[4]);
        if(unit == NULL) {
            Error::lastError = Error::UNIT_NOT_FOUND;
            return false;
        }
        
        if(!(controller->addUnit(unit, cm[2]))) {
            delete unit;
            return false;
        }
        
        return true;
    }
    
    // unit_key_create <unit_type> <label> <arg1> <arg2>
    if(std::regex_search(str, cm, Commands::regexUnitKeyCreate)) {
        Unit* unit = Unit::create(controller, cm[1], true, cm[3], cm[4]);
        if(unit == NULL) {
            Error::lastError = Error::UNIT_NOT_FOUND;
            return false;
        }
        
        if(!(controller->addUnit(unit, cm[2]))) {
            delete unit;
            return false;
        }
        
        return true;
    }
    
    // unit_delete <unit>
    if(std::regex_search(str, cm, Commands::regexUnitDelete)) {
        return controller->deleteUnit(cm[1]);
    }
    
    // unit_set_value <unit> <parameter> <value>
    if(std::regex_search(str, cm, Commands::regexUnitSetValue)) {
        Unit* unit = controller->getUnit(cm[1]);
        if(unit == NULL) {
            Error::lastError = Error::UNIT_NOT_FOUND;
            return false;
        }
        
        return unit->setValue(cm[2], cm[3]);
    }
    
    Error::lastError = Error::COMMAND_NOT_RECOGNISED;
    return false;
}
