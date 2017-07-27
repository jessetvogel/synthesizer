#ifndef program_hpp
#define program_hpp

#include <regex>

#include "interface.hpp"
#include "request.hpp"
#include "response.hpp"

class Program {
    
    static std::regex regexRestart;
    static std::regex regexExit;
    
public:
    
    static Interface* interface;
    
    
    static bool handle(Request*, Response*);
    
};

#endif
