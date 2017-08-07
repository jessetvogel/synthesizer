#include "instrument.hpp"
#include "web.hpp"
#include "error.hpp"
#include "util.hpp"

#include <iostream>

std::regex Instrument::regexInstrumentMain("^\\/instrument\\/(\\w+)\\/$");

std::string Instrument::current = "";

Interface* Instrument::interface;

bool Instrument::handle(Request* request, Response* response) {
    // Check if we are to handle this request
    std::string requestPath = request->getRequestPath();
    std::cmatch cm;
    
    // Instrument main page
    if(std::regex_match(requestPath.c_str(), cm, regexInstrumentMain)) {
        char buffer[256];
        const char* instrument = std::string(cm[1]).c_str();
        snprintf(buffer, sizeof(buffer), "../instruments/%s", instrument);
        std::string pathDirectory(buffer);
        snprintf(buffer, sizeof(buffer), "../instruments/%s/head.html", instrument);
        std::string pathHead(buffer);
        snprintf(buffer, sizeof(buffer), "../instruments/%s/body.html", instrument);
        std::string pathBody(buffer);
        
        // Check if instrument folder contains the correct files
        if(!Util::fileExists(pathBody) || !Util::fileExists(pathHead))
            return Error::respond(request, response, 500);
        
        // Check if this is a new instrument
        if(current.compare(instrument) != 0) {
            // If so, load instrument
            interface->command("play stop");
            interface->command("clear");
            interface->command("settings_set root_directory " + pathDirectory);
            interface->command("include instrument.synth");
            interface->command("play start");
            current = instrument;
        }
        
        // Construct page
        response->setHeader("Content-Type", Util::MIMEType("html"));
        response->writeStatus();
        response->writeHeaders();
        
        response->writeFile("../web/template/instrument_1.html");
        response->writeFile(pathHead);
        response->writeFile("../web/template/instrument_2.html");
        response->writeFile(pathBody);
        response->writeFile("../web/template/instrument_3.html");
        
        return true;
    }
    
    return false;
}

bool Instrument::reset() {
    current = "";
    return true;
}
