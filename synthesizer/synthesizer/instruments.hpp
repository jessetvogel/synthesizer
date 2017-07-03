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
    
    Instrument* get(std::string);
    
public:
    
    Instruments(Controller*);
    ~Instruments();
    
    // Commands
    bool create(std::string);
    bool remove(std::string);
    bool setActive(std::string, bool);
    bool setOutput(std::string, std::string);
    bool setKeyOutput(std::string, std::string);
    bool setKeyReleaseTime(std::string, double keyReleaseTime);
    
    // Status
    void printInstruments();
    
    bool apply();
    void addKeyEvent(KeyEvent*);
    
};

#endif
