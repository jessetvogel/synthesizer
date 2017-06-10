#ifndef error_hpp
#define error_hpp

#include <string>

class Error {
    
public:
    
    static std::string lastError;
    
    static std::string UNKNOWN;
    
    // Parsing errors
    static std::string COMMAND_NOT_RECOGNISED;
    static std::string INVALID_NUMBER_OF_ARGUMENTS;
    static std::string INVALID_ARGUMENT;
    static std::string EXPECTED_KEY_UNDEPENDENT;
    
    // Not found errors
    static std::string INSTRUMENT_NOT_FOUND;
    static std::string UNIT_NOT_FOUND;
    static std::string UNIT_TYPE_NOT_FOUND;
    
    // Methods
    static void print(std::string);
    static void printLastError();
};

#endif
