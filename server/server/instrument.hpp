#ifndef instrument_hpp
#define instrument_hpp

#include <regex>

#include "interface.hpp"
#include "request.hpp"
#include "response.hpp"

class Instrument {

    static std::regex regexInstrumentMain;
    
    static std::string currentInstrument;
    
public:
    
    static Interface* interface;
    
    static bool handle(Request*, Response*);
    
};

#endif
