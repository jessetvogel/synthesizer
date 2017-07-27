#ifndef status_hpp
#define status_hpp

#include <string>
#include <vector>

class Controller;

struct Message {
    
    std::string message;
    
};

class Status {

    static bool printed;
    
    static std::vector<Message> info;
    static std::vector<Message> warning;
    static std::vector<Message> error;
    static std::vector<std::string> extra;
    
    static std::string command;
    
public:
    
    inline static bool noErrors() { return error.empty(); }
    
    static void reset();
    
    static void addInfo(std::string);
    static void addWarning(std::string);
    static void addError(std::string);
    static bool addExtra(std::string);
    
    static void printInfo();
    static void printWarning();
    static void printError();
    static void printExtra(Controller*, std::string);
    
    inline static void setCommand(std::string command) { Status::command = command; }
    static void printCommand();
    
    static bool print(Controller*);
    
};

#endif
