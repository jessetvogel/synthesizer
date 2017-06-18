#ifndef unit_hpp
#define unit_hpp

#include <vector>
#include <string>

#define UNIT_DEFAULT_ID "#undefined"
#define UNIT_DEFAULT_TYPE "#undefined"

class Controller;
class Instrument;
class Parameter;

class Unit {
    
private:
    
    bool updated;
    
protected:
    
    Controller* controller;
    
    std::string id = UNIT_DEFAULT_ID;
    std::string type = UNIT_DEFAULT_TYPE;
    
    bool keyDependent;
    bool applyAlways = false;
    
    unsigned long framesPerBuffer;
    double sampleRate;
    
    inline virtual void apply(Instrument*) { };
    
public:
    
    static Unit* create(Controller*, std::string, bool, std::string, std::string);
    
    Unit(Controller*);
    ~Unit();
    
    Unit* setId(std::string);
    inline std::string getId() { return id; }
    inline std::string getType() { return type; }
    
    std::vector<Parameter*> parameters;
    
    float* output;
    
    bool setParameter(std::string, std::string);
    
    inline void reset() { updated = false; };
    void update(Instrument*);
    inline bool isKeyDependent() { return keyDependent; };
  
};

#endif
