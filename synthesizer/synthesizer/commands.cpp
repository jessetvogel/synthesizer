#include "commands.hpp"

// Patterns of types
#define REGEX_MIDI_DEVICE "\\d+"
#define REGEX_AUDIO_DEVICE "\\d+"

#define REGEX_SAMPLE_RATE "\\d+"
#define REGEX_BUFFER_SIZE "\\d+"

#define REGEX_SUSTAIN_PEDAL_POLARITY "normal|inverted"

#define REGEX_UNIT_TYPE "\\w+"
#define REGEX_UNIT_ID "\\w+"
#define REGEX_UNIT_PARAMETER "\\w+"

#define REGEX_BLOCK_ID "\\w+"
#define REGEX_BLOCK_INPUT "\\w+"
#define REGEX_BLOCK_OUTPUT "\\w+"

#define REGEX_BOOLEAN "true|false"
#define REGEX_NUMBER "[-+]?[0-9]+(?:\\.[0-9]+)?"

#define REGEX_ARGUMENTS "\\[\\w+\\s*=\\s*[\\w.\\-+]+(?:,\\s*\\w+\\s*=\\s*[\\w.\\-+]+)*\\]"

#define REGEX_PARAMETER_VALUE REGEX_UNIT_ID "|" REGEX_NUMBER "|" REGEX_BLOCK_ID ":" REGEX_BLOCK_OUTPUT

// Remove all surrounding whitespace and comments
std::regex Commands::regexPreprocess("^\\s*(.*?)\\s*(?:#.*)?$");

// Commands for MIDI
std::regex Commands::regexMidiAddDevice("^midi_add_device\\s+(" REGEX_MIDI_DEVICE ")$");
std::regex Commands::regexMidiRemoveDevice("^midi_remove_device\\s+(" REGEX_MIDI_DEVICE ")$");

// Commands for audio
std::regex Commands::regexAudioSetInputDevice("^audio_set_input_device\\s+(" REGEX_AUDIO_DEVICE ")$");
std::regex Commands::regexAudioSetOutputDevice("^audio_set_output_device\\s+(" REGEX_AUDIO_DEVICE ")$");

// Commands for settings
std::regex Commands::regexSettingsSet("^settings_set\\s+(\\w+)\\s+(" REGEX_SAMPLE_RATE ")$");

// Commands for options
std::regex Commands::regexSetSustainPedalPolarity("^options_set_sustain_pedal_polarity\\s+(" REGEX_SUSTAIN_PEDAL_POLARITY ")$");
std::regex Commands::regexSetPitchWheelRange("^options_set_pitch_wheel_range\\s+(\\d+)$");

// Commands for controller
std::regex Commands::regexPlay("^play\\s+(start|stop|toggle)$");
std::regex Commands::regexReset("^reset$");

// Commands for status
std::regex Commands::regexStatus("^status\\s+([\\w\\s]+)$");

// Commands for synths
std::regex Commands::regexInclude("^include\\s+((?:\\w+" REGEX_DIRECTORY_SEPARATOR ")*\\w+(?:.\\w+)?)$");

// Commands for units
std::regex Commands::regexUnitCreate("^unit_create\\s+(" REGEX_UNIT_TYPE ")\\s+(" REGEX_UNIT_ID ")\\s*(" REGEX_ARGUMENTS ")?$");
std::regex Commands::regexUnitRename("^unit_rename\\s+(" REGEX_UNIT_ID ")\\s+(" REGEX_UNIT_ID ")$");
std::regex Commands::regexUnitDelete("^unit_delete\\s+(" REGEX_UNIT_ID ")$");
std::regex Commands::regexUnitHide("^unit_hide\\s+(" REGEX_UNIT_ID ")$");
std::regex Commands::regexUnitSet("^unit_set\\s+(" REGEX_UNIT_ID ")\\s+(" REGEX_UNIT_PARAMETER ")\\s+(" REGEX_PARAMETER_VALUE ")$");

// Commands for blocks
std::regex Commands::regexBlockCreate("^block_create\\s+(" REGEX_BLOCK_ID ")$");
std::regex Commands::regexBlockDelete("^block_delete\\s+(" REGEX_BLOCK_ID ")$");
std::regex Commands::regexBlockAddInput("^block_add_input\\s+(" REGEX_BLOCK_ID ")\\s+(" REGEX_BLOCK_INPUT ")\\s+(" REGEX_UNIT_ID ")\\s+(" REGEX_UNIT_PARAMETER ")$");
std::regex Commands::regexBlockAddOutput("^block_add_output\\s+(" REGEX_BLOCK_ID ")\\s+(" REGEX_BLOCK_OUTPUT ")\\s+(" REGEX_UNIT_ID ")$");
std::regex Commands::regexBlockSet("^block_set\\s+(" REGEX_BLOCK_ID ")\\s+(" REGEX_BLOCK_INPUT ")\\s+(" REGEX_PARAMETER_VALUE ")$");
