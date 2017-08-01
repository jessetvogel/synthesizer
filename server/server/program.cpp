#include "program.hpp"
#include "info.hpp"
#include "instrument.hpp"

std::regex Program::regexRestart("^\\/restart$");
std::regex Program::regexExit("^\\/exit$");

Interface* Program::interface;

bool Program::handle(Request* request, Response* response) {
    // Check if we are to handle this request
    std::string requestPath = request->getRequestPath();
    std::cmatch cm;

    if(std::regex_match(requestPath.c_str(), cm, regexRestart)) {
        if(interface->restart() && Instrument::reset())
            return Info::writeJSON(request, response, "{}");
        else
            return Info::writeJSON(request, response, "{\"error\":[{\"message\":\"Unable to restart\"}]}");
    }
    
    if(std::regex_match(requestPath.c_str(), cm, regexExit)) {
        if(interface->stop())
            return Info::writeJSON(request, response, "{}");
        else
            return Info::writeJSON(request, response, "{\"error\":[{\"message\":\"Unable to restart\"}]}");
    }
    
    return false;
}