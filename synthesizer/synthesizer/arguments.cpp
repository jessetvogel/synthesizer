#include "arguments.hpp"

std::regex Arguments::regexArgument("(\\w+)\\s*=\\s*(\\w+)");

Arguments::Arguments(Controller* controller, std::string arguments) {
    // Store pointer to controller
    this->controller = controller;
    
    // Store all arguments
    std::smatch m;
    while(std::regex_search(arguments, m, regexArgument)) {
        map[m[1]] = m[2];
        arguments = m.suffix().str();
    }
}

bool Arguments::getBool(std::string key, bool standard) {
    auto position = map.find(key);
    if(position == map.end()) return standard;
    
    return position->second.compare("true") == 0;
}

int Arguments::getInteger(std::string key, int standard) {
    auto position = map.find(key);
    if(position == map.end()) return standard;
    
    return stoi(position->second);
}

double Arguments::getDouble(std::string key, double standard) {
    auto position = map.find(key);
    if(position == map.end()) return standard;
    
    return stod(position->second);
}
