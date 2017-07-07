#ifndef unit_hpp
#define unit_hpp

#include <vector>
#include <string>

#define UNIT_DEFAULT_ID "#undefined"
#define UNIT_HIDDEN_ID "#hidden"
#define UNIT_DEFAULT_TYPE "#undefined"

class Controller;
class Parameter;
class Arguments;

class Unit {
    
private:
    
    bool updated;
    
protected:
    
    Controller* controller;
    
    std::string id = UNIT_DEFAULT_ID;
    std::string type = UNIT_DEFAULT_TYPE;
    
    std::vector<Parameter*> parameters;
    
    bool keyDependent = false;
    bool hidden = false;
    bool updateParameters = true;
    
    unsigned long framesPerBuffer;
    double sampleRate;
    
    inline virtual void apply() {};
    
public:
    
    static Unit* create(Controller*, std::string, bool, std::string, std::string);
    
    Unit(Controller*);
    ~Unit();
    
    Unit* setId(std::string);
    inline std::string getId() { return id; }
    inline std::string getType() { return type; }
    
    float* output;
    
    bool setParameter(std::string, std::string);
    Parameter* getParameter(std::string);
    
    inline void reset() { updated = false; };
    void update();
    
    inline bool isKeyDependent() { return keyDependent; };
    inline void hide() { hidden = true; id = UNIT_HIDDEN_ID; }
    inline bool isHidden() { return hidden; };
  
    // Status
    void printUnit();
    
};

#endif
