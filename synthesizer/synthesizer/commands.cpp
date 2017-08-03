#include "commands.hpp"

// Patterns of types
#define REGEX_MIDI_DEVICE "\\d+"
#define REGEX_AUDIO_DEVICE "\\d+"

#define REGEX_SAMPLE_RATE "\\d+"
#define REGEX_BUFFER_SIZE "\\d+"

#define REGEX_SUSTAIN_PEDAL_POLARITY "normal|inverted"

#define REGEX_LABEL "\\w+"

#define REGEX_NODE_TYPE "\\w+"
#define REGEX_NODE_ID "\\w+"
#define REGEX_NODE_INPUT REGEX_NODE_ID "\\." REGEX_LABEL
#define REGEX_NODE_OUTPUT REGEX_NODE_ID "(?::" REGEX_LABEL ")?"

#define REGEX_BOOLEAN "true|false"
#define REGEX_NUMBER "[-+]?[0-9]+(?:\\.[0-9]+)?"

#define REGEX_OPTIONS "\\[\\w+\\s*=\\s*[\\w.\\-+]+(?:,\\s*\\w+\\s*=\\s*[\\w.\\-+]+)*\\]"

#define REGEX_VALUE REGEX_NODE_OUTPUT "|" REGEX_NUMBER "|" REGEX_LABEL

// Remove all surrounding whitespace and comments
std::regex Commands::regexPreprocess("^\\s*(.*?)\\s*(?:#.*)?$");

// Commands for MIDI
std::regex Commands::regexMidiAddDevice("^midi_add_device\\s+(" REGEX_MIDI_DEVICE ")$");
std::regex Commands::regexMidiRemoveDevice("^midi_remove_device\\s+(" REGEX_MIDI_DEVICE ")$");

// Commands for audio
std::regex Commands::regexAudioSetInputDevice("^audio_set_input_device\\s+(" REGEX_AUDIO_DEVICE ")$");
std::regex Commands::regexAudioSetOutputDevice("^audio_set_output_device\\s+(" REGEX_AUDIO_DEVICE ")$");

// Commands for settings
std::regex Commands::regexSettingsSet("^settings_set\\s+(\\w+)\\s+(" "\\S*" ")$");

// Commands for controller
std::regex Commands::regexPlay("^play\\s+(start|stop|toggle)$");
std::regex Commands::regexClear("^clear$");

// Commands for status
std::regex Commands::regexStatus("^status\\s+([\\w\\s]+)$");

// Commands for monitor
std::regex Commands::regexMonitor("^monitor\\s+(" REGEX_NODE_OUTPUT ")$");

// Commands for synths
std::regex Commands::regexInclude("^include\\s+(.*)$");

// Commands for nodes
std::regex Commands::regexNodeCreate("^node_create\\s+(" REGEX_NODE_TYPE ")\\s+(" REGEX_NODE_ID ")\\s*(" REGEX_OPTIONS ")?$");
std::regex Commands::regexNodeRename("^node_rename\\s+(" REGEX_NODE_ID ")\\s+(" REGEX_NODE_ID ")$");
std::regex Commands::regexNodeDelete("^node_delete\\s+(" REGEX_NODE_ID ")$");
std::regex Commands::regexNodeSet("^node_set\\s+(" REGEX_NODE_INPUT ")\\s+(" REGEX_VALUE ")$");
std::regex Commands::regexNodeHide("^node_hide\\s+(" REGEX_NODE_ID ")$");
std::regex Commands::regexNodeInfo("^node_info\\s+(" REGEX_NODE_ID ")$");

// Commands for custom nodes
std::regex Commands::regexNodeAddInput("^node_add_input\\s+(" REGEX_NODE_ID ")\\s+(" REGEX_LABEL ")\\s+(" REGEX_NODE_INPUT ")$");
std::regex Commands::regexNodeAddOutput("^node_add_output\\s+(" REGEX_NODE_ID ")\\s+(" REGEX_LABEL ")\\s+(" REGEX_NODE_OUTPUT ")$");
std::regex Commands::regexNodeAttach("^node_attach\\s+(" REGEX_NODE_ID ")\\s+(" REGEX_NODE_ID ")$");
