#ifndef error_hpp
#define error_hpp

#include <string>
#include <vector>

class Error {
    
    static std::vector<std::string> errors;
    
public:
    
    static std::string UNKNOWN;
    
    // IO errors
    static std::string CANNOT_OPEN_FILE;
    
    // Parsing errors
    static std::string COMMAND_NOT_RECOGNISED;
    static std::string INVALID_NUMBER_OF_ARGUMENTS;
    static std::string INVALID_ARGUMENT;
    static std::string EXPECTED_KEY_UNDEPENDENT;
    
    // Not found errors
    static std::string INSTRUMENT_NOT_FOUND;
    static std::string UNIT_NOT_FOUND;
    static std::string UNIT_TYPE_NOT_FOUND;
    static std::string UNIT_LABEL_ALREADY_USED;
    
    // Input stream errors
    static std::string INPUT_DEVICE_NOT_EXISTS;
    static std::string INPUT_ALREADY_STARTED;
    static std::string INPUT_NOT_YET_STARTED;
    static std::string INPUT_CANNOT_OPEN_INPUT;
    static std::string INPUT_CANNOT_CLOSE_INPUT;
    static std::string INPUT_READING;
    
    // Output stream errors
    static std::string OUTPUT_DEVICE_NOT_EXISTS;
    static std::string OUTPUT_ALREADY_STARTED;
    static std::string OUTPUT_NOT_YET_STARTED;
    static std::string OUTPUT_CANNOT_START_STREAM;
    static std::string OUTPUT_CANNOT_CLOSE_STREAM;
    
    // Methods
    static bool noErrors();
    static void printErrors();
    static void addError(std::string);
};

#endif
