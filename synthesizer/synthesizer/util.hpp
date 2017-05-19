#ifndef util_hpp
#define util_hpp

#include <string>
#include <regex>

class Util {

    static std::regex regexNumber;
    static std::regex regexInteger;
    
public:
    
    static bool isNumber(std::string);
    static bool isInteger(std::string);
    
};

#endif
