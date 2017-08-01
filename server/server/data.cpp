#include "data.hpp"
#include "web.hpp"
#include "info.hpp"
#include "instrument.hpp"
#include "error.hpp"
#include "util.hpp"
#include <fstream>

#include <iostream>

std::regex Data::regexSettingsSet("^\\/data\\/settings\\/set$");
std::regex Data::regexInstrumentSet("^\\/data\\/instrument\\/set$");
std::regex Data::regexInstrumentGet("^\\/data\\/instrument\\/get$");

bool Data::handle(Request* request, Response* response) {
    // Check if we are to handle this request
    std::string requestPath = request->getRequestPath();
    std::cmatch cm;
    
    // Settings
    if(std::regex_match(requestPath.c_str(), cm, regexSettingsSet)) {
        if(Data::store("../settings/settings", request->getBody()))
            return Info::writeJSON(request, response, "{}");
        else
            return Info::writeJSON(request, response, "{\"error\":[{\"message\":\"Unable to store launch data\"}]}");
    }
    
    // Instruments
    if(std::regex_match(requestPath.c_str(), cm, regexInstrumentSet)) {
        std::string instrument = Instrument::getCurrent();
        if(instrument.length() == 0)
            return Error::respond(request, response, 403); // TODO: ?
            
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "../instruments/%s/data.json", instrument.c_str());
        std::string path(buffer);
        if(!Util::fileExists(path)) {
            std::cout << "file '" << path << "' does not exist!" << std::endl;
            return Error::respond(request, response, 404);
        }
        
        if(Data::store(path, request->getBody()))
            return Info::writeJSON(request, response, "{}");
        else
            return Info::writeJSON(request, response, "{\"error\":[{\"message\":\"Unable to store settings\"}]}");
    }
    
    if(std::regex_match(requestPath.c_str(), cm, regexInstrumentGet)) {
        std::string instrument = Instrument::getCurrent();
        if(instrument.length() == 0)
            return Error::respond(request, response, 403); // TODO: ?
        
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "../instruments/%s/data.json", instrument.c_str());
        std::string path(buffer);
        if(!Util::fileExists(path)) Data::store(path, "{}");
        return Web::sendFile(request, response, path);
    }
    
    return false;
}

bool Data::store(std::string path, std::string contents) {
    std::ofstream file(path);
    if(file.fail()) return false;
    
    file << contents << std::endl;
    
    file.close();
    return true;
}