#include "instrument.hpp"
#include "web.hpp"
#include "error.hpp"
#include "util.hpp"

#include <iostream>

std::regex Instrument::regexInstrumentMain("^\\/instrument\\/(\\w+)\\/$");

Interface* Instrument::interface;

bool Instrument::handle(Request* request, Response* response) {
    // Check if we are to handle this request
    std::string requestURI = request->getRequestURI();
    std::cmatch cm;
    
    // Instrument main page
    if(std::regex_match(requestURI.c_str(), cm, regexInstrumentMain)) {
        char buffer[256];
        const char* instrument = std::string(cm[1]).c_str();
        snprintf(buffer, sizeof(buffer), "../instruments/%s/head.html", instrument);
        std::string pathHead(buffer);
        snprintf(buffer, sizeof(buffer), "../instruments/%s/body.html", instrument);
        std::string pathBody(buffer);
        snprintf(buffer, sizeof(buffer), "../instruments/%s/instrument.synth", instrument);
        std::string pathInstrument(buffer);
        
        // Check if instrument folder contains the correct files
        if(!Util::fileExists(pathBody) || !Util::fileExists(pathHead) || !Util::fileExists(pathInstrument))
            return Error::respond(request, response, 500);
        
        // Load instrument
        interface->command("play stop");
        interface->command("clear");
        interface->command("include " + pathInstrument);
        interface->command("play start");
        
        // If so, construct page
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
