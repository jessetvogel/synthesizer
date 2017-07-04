#ifndef instruments_hpp
#define instruments_hpp

#include <vector>
#include <string>
#include <mutex>

class Controller;
class Instrument;
class KeyEvent;

class Instruments {

    Controller* controller;
    
    std::vector<Instrument*> instruments;
    
    std::mutex mutex;
    
    Instrument* getInstrument(std::string);
    
public:
    
    Instruments(Controller*);
    ~Instruments();
    
    bool apply();
    void addKeyEvent(KeyEvent*);
    
    // Commands
    bool create(std::string);
    bool destroy(std::string);
    bool setActive(std::string, bool);
    bool setOutput(std::string, std::string);
    bool setKeyOutput(std::string, std::string);
    bool setKeyReleaseTime(std::string, double keyReleaseTime);
    
    // Status
    void printInstruments();
    
};

#endif
