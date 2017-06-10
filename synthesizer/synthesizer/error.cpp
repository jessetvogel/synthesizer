#include <iostream>

#include "error.hpp"

std::string Error::lastError = "";

// Default errors
std::string Error::UNKNOWN = "Unknown error";

// Parsing errors
std::string Error::COMMAND_NOT_RECOGNISED = "The given command was not recognised";
std::string Error::INVALID_NUMBER_OF_ARGUMENTS = "Invalid number of arguments given";
std::string Error::INVALID_ARGUMENT = "Invalid argument given";
std::string Error::EXPECTED_KEY_UNDEPENDENT = "Given unit is key dependent, which is not allowed";

// Not found errors
std::string Error::INSTRUMENT_NOT_FOUND = "The given instrument was not found";
std::string Error::UNIT_NOT_FOUND = "The given unit was not found";
std::string Error::UNIT_TYPE_NOT_FOUND = "The given unit type does not exist";


void Error::print(std::string message) {
    // In case of an empty error, say the error is unknown
    if(message.empty()) print(UNKNOWN);
    
    // Print error
    std::cout << message << std::endl;
    
    // Set lastError to default
    lastError = "";
}

void Error::printLastError() {
    print(lastError);
}
