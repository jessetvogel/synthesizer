#include "keyevent.hpp"

#include <iostream>

bool KeyEvent::idUsed[MAX_AMOUNT_OF_IDS] = {};

KeyEvent::KeyEvent() {
    // Look for an unused id
    for(int i = 0;i < MAX_AMOUNT_OF_IDS; ++i) {
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
