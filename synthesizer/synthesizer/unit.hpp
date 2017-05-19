#ifndef unit_hpp
#define unit_hpp

#include <string>
#include "keyunit.hpp"

class Controller;

class Unit : public KeyUnit {
    
public:
        
    static Unit* create(Controller*, std::string);
    static bool set(Controller*, Unit**, std::string);
    
};

#endif
