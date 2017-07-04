#ifndef units_hpp
#define units_hpp

#include <vector>
#include <unordered_map>
#include <mutex>

class Controller;
class Unit;
class UnitConstant;
class UnitParameter;

class Units {

    Controller* controller;
    
    std::vector<Unit*> units;
    std::vector<UnitConstant*> constants;
    std::unordered_map<int, UnitParameter*> parameters;
    
    std::mutex mutexUnits;
    std::mutex mutexConstants;
    std::mutex mutexParameters;
    
public:
    
    Units(Controller*);
    ~Units();
    
    bool addUnit(Unit*);
    Unit* getUnit(std::string);
    bool deleteUnit(Unit*);
    
    UnitConstant* createConstant(double);
    bool deleteConstant(UnitConstant*);
    bool isConstant(Unit*);
    
    bool addParameter(UnitParameter*, int);
    UnitParameter* getParameter(int);
    bool deleteParameter(int);
    
    void resetUnits();
    void resetUnitsKeyDependent();
    
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
