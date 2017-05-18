#include <fstream>
#include <regex>

#include "parser.hpp"
#include "commands.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "unit.hpp"
#include "keyunit.hpp"

#include "log.hpp"

std::regex Parser::regexNeglect("^(\\s*|\\s*#.*)$");

Parser::Parser(Controller* controller, std::string filename) {
    // Store pointer to controller
    this->controller = controller;
    
    // Read file line by line, and parse them
    std::ifstream input(filename);
    std::string line;
    while(std::getline(input, line)) {
        if(!parse(line)) {
            Log::error("Unable to parse to parse the following line:");
            Log::error(line.c_str());
            Log::error("");
        }
    }
    input.close();
}

bool Parser::parse(std::string line) {
    const char* str = line.c_str();
    std::cmatch cm;

    // Empty lines or comments
    if(std::regex_search(str, cm, regexNeglect))
        return true;
    
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
        
        instrument->setOutput(unit);
        return true;
    }
    
    // instrument_key_set_output <instrument> <key_unit>
    if(std::regex_search(str, cm, Commands::regexInstrumentKeySetOutput)) {
        Instrument* instrument = controller->getInstrument(cm[1]);
        if(instrument == NULL) return false;
        KeyUnit* keyUnit = controller->getKeyUnit(cm[2]);
        if(keyUnit == NULL) return false;
        
        instrument->setKeyOutput(keyUnit);
        return true;
    }
    
    // instrument_key_set_release_time <instrument> <seconds>
    if(std::regex_search(str, cm, Commands::regexInstrumentSetReleaseTime)) {
        Instrument* instrument = controller->getInstrument(cm[1]);
        if(instrument == NULL) return false;

        instrument->setReleaseTime(stod(cm[2]));
        return true;
    }
    
    // unit_create <unit_type> <label>
    if(std::regex_search(str, cm, Commands::regexUnitCreate)) {
        Unit* unit = Unit::create(controller, cm[1]);
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
        
        return unit->setValue(cm[1], cm[2]);
    }
    
    // key_unit_create <type> <label>
    if(std::regex_search(str, cm, Commands::regexKeyUnitCreate)) {
        KeyUnit* keyUnit = KeyUnit::create(controller, cm[1]);
        if(keyUnit == NULL) return false;
            
        if(!(controller->addKeyUnit(keyUnit, cm[2]))) {
            delete keyUnit;
            return false;
        }
        
        return true;
    }
    
    // key_unit_delete <label>
    if(std::regex_search(str, cm, Commands::regexKeyUnitDelete)) {
        return controller->deleteKeyUnit(cm[1]);
    }
    
    // key_unit_set_value <key_unit> <parameter> <value>
    if(std::regex_search(str, cm, Commands::regexKeyUnitSetValue)) {
        KeyUnit* keyUnit = controller->getKeyUnit(cm[1]);
        
        if(keyUnit == NULL) return false;
        
        return keyUnit->setValue(cm[2], cm[3]);
    }
    
    return false;
}
