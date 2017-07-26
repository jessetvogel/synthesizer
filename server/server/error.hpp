#ifndef error_hpp
#define error_hpp

#include "request.hpp"
#include "response.hpp"

class Error {

public:
    
    static bool respond(Request*, Response*, int code);
    
};

#endif
