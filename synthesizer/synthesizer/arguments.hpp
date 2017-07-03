#ifndef arguments_hpp
#define arguments_hpp

#include <string>
#include <unordered_map>
#include <regex>

class Controller;

class Arguments {

    Controller* controller;
    
    std::unordered_map<std::string, std::string> map;
    
    static std::regex regexArgument;
    
public:
    
    Arguments(Controller*, std::string);
    
    bool getBool(std::string, bool);
    int getInteger(std::string, int);
    double getDouble(std::string, double);
    
};

#endif
