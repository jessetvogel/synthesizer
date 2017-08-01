#ifndef options_hpp
#define options_hpp

#include <string>
#include <unordered_map>
#include <regex>

class Controller;

class Options {

    Controller* controller;
    
    std::unordered_map<std::string, std::string> map;
    
    static std::regex regexOption;
    
public:
    
    Options(Controller*, std::string);
    
    bool getBool(std::string, bool);
    int getInteger(std::string, int);
    double getDouble(std::string, double);
    std::string getString(std::string, std::string);
    
};

#endif
