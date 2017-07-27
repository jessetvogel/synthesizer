#ifndef info_hpp
#define info_hpp

#include <regex>

#include "request.hpp"
#include "response.hpp"

class Info {

    static std::regex regexInfo;
    
public:
    
    static bool handle(Request*, Response*);
    static bool writeJSON(Request*, Response*, std::string);

};

#endif
