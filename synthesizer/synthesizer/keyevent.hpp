#ifndef keyevent_hpp
#define keyevent_hpp

#define MAX_AMOUNT_OF_IDS (256)

class KeyEvent {
    
    static bool idUsed[MAX_AMOUNT_OF_IDS];
    
public:

    KeyEvent();
    ~KeyEvent();
    
    enum Stage { Off, Press, Sustain, Released };
    
    int id;
    
    unsigned char key;
    Stage stage;
    double velocity;
    double frequency;
    double duration;
    double release;
    
};

#endif
