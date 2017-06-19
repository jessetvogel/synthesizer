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
    
public:
    
    Instruments(Controller*);
    ~Instruments();
    
    bool add(Instrument*);
    bool remove(Instrument*);
    Instrument* get(std::string);
    
    bool apply();
    
    void addKeyEvent(KeyEvent*);
    
    void printInstruments();
    
};

#endif
