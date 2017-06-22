#include "commands.hpp"

// Remove all surrounding whitespace and comments
std::regex Commands::regexPreprocess("^\\s*(.*?)\\s*(?:#.*)?$");

// Commands for MIDI
std::regex Commands::regexMidiAddDevice("^midi_add_device\\s+(\\d+)$");
std::regex Commands::regexMidiRemoveDevice("^midi_remove_device\\s+(\\d+)$");

// Commands for audio
std::regex Commands::regexAudioSetInputDevice("^audio_set_input_device\\s+(\\d+)$");
std::regex Commands::regexAudioSetOutputDevice("^audio_set_output_device\\s+(\\d+)$");

// Commands for settings
std::regex Commands::regexSetSampleRate("^settings_set_sample_rate\\s+(\\d+)$");
std::regex Commands::regexSetBufferSize("^settings_set_buffer_size\\s+(\\d+)$");
std::regex Commands::regexSetRootDirectory("^settings_set_root_directory\\s+(.+)$");

// Commands for options
std::regex Commands::regexSetSustainPedalPolarity("^options_set_sustain_pedal_polarity\\s+(normal|inverted)$");
std::regex Commands::regexSetPitchWheelRange("^options_set_pitch_wheel_range\\s+(\\d+)$");

// Commands for controller
std::regex Commands::regexStart("^start$");
std::regex Commands::regexStop("^stop$");
std::regex Commands::regexReset("^reset$");

// Commands for status
std::regex Commands::regexStatus("^status\\s+([\\w\\s]+)$");

// Commands for synths
std::regex Commands::regexInclude("^include\\s+((?:\\w+" REGEX_DIRECTORY_SEPARATOR ")*\\w+(?:.\\w+)?)$");

std::regex Commands::regexInstrumentCreate("^instrument_create\\s+(\\w+)$");
std::regex Commands::regexInstrumentDelete("^instrument_delete\\s+(\\w+)$");
std::regex Commands::regexInstrumentSetOutput("^instrument_set_output\\s+(\\w+)\\s+(\\w+)$");
std::regex Commands::regexInstrumentSetKeyOutput("^instrument_set_key_output\\s+(\\w+)\\s+(\\w+)$");
std::regex Commands::regexInstrumentSetKeyReleaseTime("^instrument_set_key_release_time\\s+(\\w+)\\s+([-+]?[0-9]+(?:\\.[0-9]+)?)$");

std::regex Commands::regexUnitCreate("^unit_create\\s+(\\w+)\\s+(\\w+)(?:\\s+([\\w.]+))?(?:\\s+([\\w.]+))?$"); // Currently allows for two arguments
std::regex Commands::regexUnitKeyCreate("^unit_key_create\\s+(\\w+)\\s+(\\w+)(?:\\s+([\\w.]+))?(?:\\s+([\\w.]+))?$"); // Currently allows for two arguments
std::regex Commands::regexUnitRename("^unit_rename\\s+(\\w+)\\s+(\\w+)$");
std::regex Commands::regexUnitDelete("^unit_delete\\s+(\\w+)$");
std::regex Commands::regexUnitSetValue("^unit_set_value\\s+(\\w+)\\s+(\\w+)\\s+(\\w+|[-+]?[0-9]+(?:\\.[0-9]+)?)$");
