#ifndef unitfactory_hpp
#define unitfactory_hpp

#include <string>

class Controller;
class Units;
class Unit;
class Arguments;

class UnitFactory {
    
public:
    
    static void createDefaultUnits(Controller*, Units*);
    static Unit* create(Controller*, std::string, std::string, Arguments);
    
};

#endif
