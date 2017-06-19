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
    
    bool add(Unit*);
    bool remove(Unit*);
    Unit* get(std::string);
    
    UnitConstant* createConstant(double);
    bool deleteConstant(UnitConstant*);
    bool isConstant(Unit*);
    
    bool addParameter(UnitParameter*, int);
    UnitParameter* getParameter(int);
    bool deleteParameter(int);
    
    void resetUnits();
    void resetUnitsKeyDependent();
    
    void printUnits();
};

#endif
