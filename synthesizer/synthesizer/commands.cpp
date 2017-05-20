#include "commands.hpp"

std::regex Commands::regexInclude("^include\\s+((?:\\w+" REGEX_DIRECTORY_SEPARATOR ")*\\w+(?:.\\w+)?)$");

std::regex Commands::regexInstrumentCreate("^instrument_create\\s+(\\w+)$");
std::regex Commands::regexInstrumentDelete("^instrument_delete\\s+(\\w+)$");
std::regex Commands::regexInstrumentSetOutput("^instrument_set_output\\s+(\\w+)\\s+(\\w+)$");
std::regex Commands::regexInstrumentSetKeyOutput("^instrument_set_key_output\\s+(\\w+)\\s+(\\w+)$");
std::regex Commands::regexInstrumentSetKeyReleaseTime("^instrument_set_key_release_time\\s+(\\w+)\\s+([-+]?[0-9]+(?:\\.[0-9]+)?)$");

std::regex Commands::regexUnitCreate("^unit_create\\s+(\\w+)\\s+(\\w+)$");
std::regex Commands::regexUnitKeyCreate("^unit_key_create\\s+(\\w+)\\s+(\\w+)$");
std::regex Commands::regexUnitDelete("^unit_delete\\s+(\\w+)$");
std::regex Commands::regexUnitSetValue("^unit_set_value\\s+(\\w+)\\s+(\\w+)\\s+(\\w+|[-+]?[0-9]+(?:\\.[0-9]+)?)$");

// TODO: remove this
std::regex Commands::regexPrint("^print\\s+(.+)$");