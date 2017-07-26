#include "error.hpp"
#include "util.hpp"

bool Error::respond(Request* request, Response* response, int code) {
    // Determine status belonging to this HTTP code
    std::string status;
    switch(code) {
        case 400: status = "400 Bad Request"; break;
        case 403: status = "403 Forbidden"; break;
        case 404: status = "404 Not Found"; break;

        default:
            code = 500;
            status = "500 Internal Server Error"; break;
    }
    
    // Respond with corresponding error document
    response->setStatus(status);
    response->setHeader("Content-Type", Util::MIMEType("html"));
    
    response->writeStatus();
    response->writeHeaders();
    char path[128];
    sprintf(path, "../web/error/%d.html", code);
    response->writeFile(path);

    return true;
}
