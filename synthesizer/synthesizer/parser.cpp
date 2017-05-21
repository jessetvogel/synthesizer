#include <fstream>
#include <regex>

#include "parser.hpp"
#include "commands.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "unit.hpp"

#include "log.hpp"

std::regex Parser::regexNeglect("^(\\s*|\\s*#.*)$");

Parser::Parser(Controller* controller, std::string filepath) {
    // Store pointer to controller
    this->controller = controller;
    
    // Store filepath
    this->filepath = filepath;
    
    // Determine directory
    directory = filepath.substr(0, filepath.find_last_of(DIRECTORY_SEPARATOR));
}

bool Parser::parse() {
    // Read file line by line, and parse them
    int lineNumber = 1;
    std::ifstream input(filepath);
    std::string line;
    while(std::getline(input, line)) {
        if(!parseLine(line)) {
            char message[32];
            sprintf(message, "An error occured in line %d", lineNumber);
            Log::error(message);
            Log::error(line.c_str());
            return false;
        }
        ++ lineNumber;
    }
    input.close();
    return true;
}

bool Parser::parseLine(std::string line) {
    const char* str = line.c_str();
    std::cmatch cm;

    // Empty lines or comments
    if(std::regex_search(str, cm, regexNeglect))
        return true;
    
    // include <filename>
    if(std::regex_search(str, cm, Commands::regexInclude)) {
        Parser parser(controller, directory + DIRECTORY_SEPARATOR + std::string(cm[1]));
        return parser.parse();
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
        if(instrument == NULL) return false;
        Unit* unit = controller->getUnit(cm[2]);
        if(unit == NULL) return false;
        
        return instrument->setOutput(unit);
    }
    
    // instrument_set_key_output <instrument> <key_unit>
    if(std::regex_search(str, cm, Commands::regexInstrumentSetKeyOutput)) {
        Instrument* instrument = controller->getInstrument(cm[1]);
        if(instrument == NULL) return false;
        Unit* unit = controller->getUnit(cm[2]);
        if(unit == NULL) return false;
        
        return instrument->setKeyOutput(unit);
    }
    
    // instrument_set_key_release_time <instrument> <seconds>
    if(std::regex_search(str, cm, Commands::regexInstrumentSetKeyReleaseTime)) {
        Instrument* instrument = controller->getInstrument(cm[1]);
        if(instrument == NULL) return false;

        instrument->setKeyReleaseTime(stod(cm[2]));
        return true;
    }
    
    // unit_create <unit_type> <label>
    if(std::regex_search(str, cm, Commands::regexUnitCreate)) {
        Unit* unit = Unit::create(controller, cm[1], false);
        if(unit == NULL) return false;
        
        if(!(controller->addUnit(unit, cm[2]))) {
            delete unit;
            return false;
        }
        
        return true;
    }
    
    // unit_key_create <unit_type> <label>
    if(std::regex_search(str, cm, Commands::regexUnitKeyCreate)) {
        Unit* unit = Unit::create(controller, cm[1], true);
        if(unit == NULL) return false;
        
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
        if(unit == NULL) return false;
        
        return unit->setValue(cm[2], cm[3]);
    }
        
    
    
    
    
    
    
    // TODO: remove this
    // print <message>
    if(std::regex_search(str, cm, Commands::regexPrint)) {
        Log::output(std::string(cm[1]).c_str());
        return true;
    }
    
    return false;
}
