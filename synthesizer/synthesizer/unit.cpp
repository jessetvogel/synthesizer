#include "unit.hpp"

void Unit::apply() { }

bool Unit::setValue(std::string parameter, std::string value) {
    return true;
};

void Unit::reset() {
    updated = false;
};

void Unit::update() {
    if(updated) return;
    updated = true;
    apply();
};

Unit* Unit::create(Controller* controller, std::string type) {
    return NULL;
}
