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
    if(std::regex_search(str, cm, Commands::regexMidiAddDevice)) {
        return controller->getMIDIDevices()->add(stoi(cm[1]));
    }
    
    // midi_remove_device <input_device>
    if(std::regex_search(str, cm, Commands::regexMidiRemoveDevice)) {
        return controller->getMIDIDevices()->remove(stoi(cm[1]));
    }
    
    // Audio
    
    // audio_set_input_device <input_device>
    if(std::regex_search(str, cm, Commands::regexAudioSetInputDevice)) {
        return controller->getAudioDevices()->setInputDeviceId(stoi(cm[1]));
    }
    
    // audio_set_output_device <input_device>
    if(std::regex_search(str, cm, Commands::regexAudioSetOutputDevice)) {
        return controller->getAudioDevices()->setOutputDeviceId(stoi(cm[1]));
    }
    
    // Options
    
    // options_set_sustain_pedal_polarity <normal|inverted>
    if(std::regex_search(str, cm, Commands::regexSetSustainPedalPolarity)) {
        controller->getOptions()->sustainPedalPolarity = (cm[1].compare("inverted") == 0);
        return true;
    }
    
    // options_set_pitch_wheel_range <semitones>
    if(std::regex_search(str, cm, Commands::regexSetPitchWheelRange)) {
        controller->getOptions()->pitchWheelRange = stod(cm[1]);
        return true;
    }
    
    // Controller
    
    // start
    if(std::regex_search(str, cm, Commands::regexStart)) {
        return controller->start();
    }
    
    // stop
    if(std::regex_search(str, cm, Commands::regexStop)) {
        return controller->stop();
    }
    
    // reset
    if(std::regex_search(str, cm, Commands::regexReset)) {
        // TODO!!
//        return controller->reset();
        return true;
    }
    
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
    
    // instrument_create <label>
    if(std::regex_search(str, cm, Commands::regexInstrumentCreate)) {
        Instrument* instrument = new Instrument(controller);
        instrument->setId(cm[1]);
        if(!(controller->getInstruments()->add(instrument))) {
            delete instrument;
            return false;
        };
        return true;
    }
    
    // instrument_delete <instrument>
    if(std::regex_search(str, cm, Commands::regexInstrumentDelete)) {
        Instrument* instrument = controller->getInstruments()->get(cm[1]);
        if(instrument == NULL) {
            Error::addError(Error::INSTRUMENT_NOT_FOUND);
            return false;
        }
        return controller->getInstruments()->remove(instrument);
    }
    
    // instrument_set_output <instrument> <unit>
    if(std::regex_search(str, cm, Commands::regexInstrumentSetOutput)) {
        Instrument* instrument = controller->getInstruments()->get(cm[1]);
        if(instrument == NULL) {
            Error::addError(Error::INSTRUMENT_NOT_FOUND);
            return false;
        }
        Unit* unit = controller->getUnits()->get(cm[2]);
        if(unit == NULL) {
            Error::addError(Error::UNIT_NOT_FOUND);
            return false;
        }
        return instrument->setOutput(unit);
    }
    
    // instrument_set_key_output <instrument> <key_unit>
    if(std::regex_search(str, cm, Commands::regexInstrumentSetKeyOutput)) {
        Instrument* instrument = controller->getInstruments()->get(cm[1]);
        if(instrument == NULL) {
            Error::addError(Error::INSTRUMENT_NOT_FOUND);
            return false;
        }
        Unit* unit = controller->getUnits()->get(cm[2]);
        if(unit == NULL) {
            Error::addError(Error::UNIT_NOT_FOUND);
            return false;
        }
        return instrument->setKeyOutput(unit);
    }
    
    // instrument_set_key_release_time <instrument> <seconds>
    if(std::regex_search(str, cm, Commands::regexInstrumentSetKeyReleaseTime)) {
        Instrument* instrument = controller->getInstruments()->get(cm[1]);
        if(instrument == NULL) {
            Error::addError(Error::INSTRUMENT_NOT_FOUND);
            return false;
        }

        instrument->setKeyReleaseTime(stod(cm[2]));
        return true;
    }
    
    // unit_create <unit_type> <label> <arg1> <arg2>
    if(std::regex_search(str, cm, Commands::regexUnitCreate)) {
        Unit* unit = Unit::create(controller, cm[1], false, cm[3], cm[4]);
        if(unit == NULL)
            return false;
        
        unit->setId(cm[2]);
        if(!(controller->getUnits()->add(unit))) {
            delete unit;
            Error::addError(Error::UNIT_LABEL_ALREADY_USED);
            return false;
        }
        
        return true;
    }
    
    // unit_key_create <unit_type> <label> <arg1> <arg2>
    if(std::regex_search(str, cm, Commands::regexUnitKeyCreate)) {
        Unit* unit = controller->getUnits()->get(cm[2]);
        if(unit != NULL) {
            Error::addError(Error::UNIT_LABEL_ALREADY_USED);
            return false;
        }
        
        unit = Unit::create(controller, cm[1], true, cm[3], cm[4]);
        if(unit == NULL) {
//            Error::lastError = Error::UNIT_NOT_FOUND;
            return false;
        }
        
        unit->setId(cm[2]);
        controller->getUnits()->add(unit);
        return true;
    }
    
    // unit_rename <unit> <label>
    if(std::regex_search(str, cm, Commands::regexUnitRename)) {
        Unit* unit = controller->getUnits()->get(cm[1]);
        if(unit == NULL) {
            Error::addError(Error::UNIT_NOT_FOUND);
            return false;
        }
        
        Unit* unitNew = controller->getUnits()->get(cm[2]);
        if(unitNew != NULL) {
            Error::addError(Error::UNIT_LABEL_ALREADY_USED);
            return false;
        }
        
        return unit->setId(cm[2]);
    }
    
    // unit_delete <unit>
    if(std::regex_search(str, cm, Commands::regexUnitDelete)) {
        Unit* unit = controller->getUnits()->get(cm[1]);
        if(unit == NULL) {
            Error::addError(Error::UNIT_NOT_FOUND);
            return false;
        }
        return controller->getUnits()->remove(unit);
    }
    
    // unit_set_value <unit> <parameter> <value>
    if(std::regex_search(str, cm, Commands::regexUnitSetValue)) {
        Unit* unit = controller->getUnits()->get(cm[1]);
        if(unit == NULL) {
            Error::addError(Error::UNIT_NOT_FOUND);
            return false;
        }
        return unit->setParameter(cm[2], cm[3]);
    }
    
    Error::addError(Error::COMMAND_NOT_RECOGNISED);
    return false;
}
