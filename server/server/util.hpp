#ifndef util_hpp
#define util_hpp

#include <string>

class Util {
    
public:
    
    static bool isFile(std::string);
    static bool isInteger(std::string);
    static bool fileExists(std::string);
    static std::string MIMEType(std::string);
    static std::string toLowerCase(std::string);
    
};

#endif
