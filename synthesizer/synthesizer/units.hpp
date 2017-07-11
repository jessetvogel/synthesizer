#ifndef units_hpp
#define units_hpp

#include <vector>
#include <unordered_map>
#include <mutex>
#include "keyevent.hpp"

class Controller;
class Unit;
class UnitCollector;
class UnitAudioOutput;
class UnitConstant;
class UnitParameter;

class Units {
    
    Controller* controller;
    
    std::vector<Unit*> units;
    std::vector<UnitCollector*> collectors;
    std::vector<UnitAudioOutput*> audioOutputs;
    std::vector<UnitConstant*> constants;
    std::unordered_map<int, UnitParameter*> parameters;
    
    std::mutex mutex;
    
public:
    
    Units(Controller*);
    ~Units();
    
    bool addUnit(Unit*);
    Unit* getUnit(std::string);
    
    UnitConstant* createConstant(double);
    bool deleteConstant(UnitConstant*);
    
    bool addCollector(UnitCollector*);
    bool removeCollector(UnitCollector*);
    
    bool addAudioOutput(UnitAudioOutput*);
    bool removeAudioOutput(UnitAudioOutput*);
    
    bool addParameter(UnitParameter*);
    UnitParameter* getParameter(int);
    bool removeParameter(UnitParameter*);
    
    KeyEvent* currentKey;
    void addKeyEvent(KeyEvent*);
    
    void resetUnits();
    void resetUnitsKeyDependent();
    
    bool apply();
    
    // Commands
    bool create(std::string, std::string, std::string);
    bool destroy(std::string);
    bool rename(std::string, std::string);
    bool hide(std::string);
    bool set(std::string, std::string, std::string);
    
    // Status
    void printUnits();
    
};

#endif
