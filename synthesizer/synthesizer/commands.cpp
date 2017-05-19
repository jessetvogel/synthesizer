#include "commands.hpp"

std::regex Commands::regexInstrumentCreate("^instrument_create\\s+(\\w+)$");
std::regex Commands::regexInstrumentDelete("^instrument_delete\\s+(\\w+)$");
std::regex Commands::regexInstrumentSetOutput("^instrument_set_output\\s+(\\w+)\\s+(\\w+)$");
std::regex Commands::regexInstrumentKeySetOutput("^instrument_key_set_output\\s+(\\w+)\\s+(\\w+)$");
std::regex Commands::regexInstrumentSetReleaseTime("^instrument_key_set_release_time\\s+(\\w+)\\s+([-+]?[0-9]+(?:\\.[0-9]+)?)$");

std::regex Commands::regexUnitCreate("^unit_create\\s+(\\w+)\\s+(\\w+)$");
std::regex Commands::regexUnitDelete("^unit_delete\\s+(\\w+)$");
std::regex Commands::regexUnitSetValue("^unit_set_value\\s+(\\w+)\\s+(\\w+)\\s+(\\w+|[-+]?[0-9]+(?:\\.[0-9]+)?)$");

std::regex Commands::regexKeyUnitCreate("^key_unit_create\\s+(\\w+)\\s+(\\w+)$");
std::regex Commands::regexKeyUnitDelete("^key_unit_delete\\s+(\\w+)$");
std::regex Commands::regexKeyUnitSetValue("^key_unit_set_value\\s+(\\w+)\\s+(\\w+)\\s+(\\w+|[-+]?[0-9]+(?:\\.[0-9]+)?)$");
