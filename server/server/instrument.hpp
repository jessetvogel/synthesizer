#ifndef instrument_hpp
#define instrument_hpp

#include <regex>

#include "interface.hpp"
#include "request.hpp"
#include "response.hpp"

class Instrument {

    static std::regex regexInstrumentMain;
    
    static std::string current;
    
public:
    
    static Interface* interface;
    
    inline static std::string getCurrent() { return current; };
    
    static bool handle(Request*, Response*);
    static bool reset();
};

#endif
