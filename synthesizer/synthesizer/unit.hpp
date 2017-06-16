#ifndef unit_hpp
#define unit_hpp

#include <vector>
#include <string>

class Controller;
class Instrument;
class Parameter;

class Unit {
    
private:
    
    bool updated;
    
protected:
    
    Controller* controller;
    bool keyDependent;
    bool applyAlways = false;
    
    inline virtual void apply(Instrument*) { };
    
public:
    
    static Unit* create(Controller*, std::string, bool, std::string, std::string);
    ~Unit();
    
    std::string id = "undefined";
    std::string type = "undefined";
    std::vector<Parameter*> parameters;
    
    float* output;
    
    bool setParameter(std::string, std::string);
    
    inline void reset() { updated = false; };
    void update(Instrument*);
    inline bool isKeyDependent() { return keyDependent; };
    inline std::string getId() { return id; }
    inline std::string getType() { return type; }
    
};

#endif
