#ifndef api_hpp
#define api_hpp

#include <regex>

#include "interface.hpp"
#include "request.hpp"
#include "response.hpp"

class API {

    static std::regex regexAPI;
    
public:
    
    static Interface* interface;
    
    static bool handle(Request*, Response*);
    
};

#endif
