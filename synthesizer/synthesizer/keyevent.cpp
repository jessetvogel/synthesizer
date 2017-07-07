#include "keyevent.hpp"

#include <iostream>

bool KeyEvent::idUsed[SETTINGS_POLYPHONY] = {};

KeyEvent::KeyEvent() {
    // Look for an unused id
    for(int i = 0;i < SETTINGS_POLYPHONY; ++i) {
        if(!idUsed[i]) {
            id = i;
            idUsed[i] = true;
            return;
        }
    }
}

KeyEvent::~KeyEvent() {
    idUsed[id] = false;
}
