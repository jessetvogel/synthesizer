#include "options.hpp"
#include "util.hpp"

std::regex Options::regexOption("(\\w+)\\s*=\\s*([\\w.]+)");

Options::Options(Controller* controller, std::string args) {
    // Store pointer to controller
    this->controller = controller;
    
    // Store all options
    std::smatch m;
    while(std::regex_search(args, m, regexOption)) {
        map[m[1]] = m[2];
        args = m.suffix().str();
    }
}

bool Options::getBool(std::string key, bool standard) {
    auto position = map.find(key);
    if(position == map.end()) return standard;
    
    return position->second.compare("true") == 0;
}

int Options::getInteger(std::string key, int standard) {
    auto position = map.find(key);
    if(position == map.end()) return standard;
    
    std::string value = position->second;
    if(!Util::isInteger(value)) return standard;
    
    return stoi(value);
}

double Options::getDouble(std::string key, double standard) {
    auto position = map.find(key);
    if(position == map.end()) return standard;
    
    return stod(position->second);
}
