#include <iostream>

#include "error.hpp"

std::vector<std::string> Error::errors;

// Default errors
std::string Error::UNKNOWN = "Unknown error";

// IO errors
std::string Error::CANNOT_OPEN_FILE = "Error in opening file";

// Parsing errors
std::string Error::COMMAND_NOT_RECOGNISED = "The given command was not recognised";
std::string Error::INVALID_NUMBER_OF_ARGUMENTS = "Invalid number of arguments given";
std::string Error::INVALID_ARGUMENT = "Invalid argument given";
std::string Error::EXPECTED_KEY_UNDEPENDENT = "Given unit is key dependent, which is not allowed";

// Not found errors
std::string Error::INSTRUMENT_NOT_FOUND = "The given instrument was not found";
std::string Error::UNIT_NOT_FOUND = "The given unit was not found";
std::string Error::UNIT_TYPE_NOT_FOUND = "The given unit type does not exist";
std::string Error::UNIT_LABEL_ALREADY_USED = "The given label is already used";

// Input stream errors
std::string Error::INPUT_DEVICE_NOT_EXISTS = "Input device does not exist";
std::string Error::INPUT_ALREADY_STARTED = "Input device already started";
std::string Error::INPUT_NOT_YET_STARTED = "Input device was not yet started";
std::string Error::INPUT_CANNOT_OPEN_INPUT = "Unable to open input";
std::string Error::INPUT_CANNOT_CLOSE_INPUT = "Unable to close input";
std::string Error::INPUT_READING = "Was not able to read from input device";

// Output stream errors
std::string Error::OUTPUT_DEVICE_NOT_EXISTS = "Output device does not exist";
std::string Error::OUTPUT_ALREADY_STARTED = "Output device already started";
std::string Error::OUTPUT_NOT_YET_STARTED = "Output device was not yet started";
std::string Error::OUTPUT_CANNOT_START_STREAM = "Unable to open output stream";
std::string Error::OUTPUT_CANNOT_CLOSE_STREAM = "Unable to close output stream";

void Error::addError(std::string message) {
    errors.push_back(message);
}

void Error::printErrors() {
    // Print error
    std::cout << "\"errors\": [";
    
    bool comma = false;
    for(auto it = errors.begin(); it != errors.end(); ++it) {
        if(comma) std::cout << ","; else comma = true; // Make sure errors get separated by commas
        std::cout << "'" << (*it) << "'";
    }
    
    std::cout << "]";
    
    // Clear errors
    errors.clear();
}

bool Error::noErrors() {
    return errors.empty();
}
