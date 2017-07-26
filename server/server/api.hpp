#ifndef api_hpp
#define api_hpp

#include <regex>

#include "interface.hpp"
#include "request.hpp"
#include "response.hpp"

class API {

    static std::regex regexRequestURI;
    
public:
    
    static Interface* interface;
    
    static bool handle(Request*, Response*);
    
};

#endif
