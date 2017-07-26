#include "headers.hpp"
#include "response.hpp"

void Headers::set(std::string field, std::string value) {
    headers[field] = value;
}

std::string Headers::get(std::string field) {
    auto position = headers.find(field);
    if(position == headers.end()) return NULL;
    return position->second;
}

bool Headers::write(Response* response) {
    for(auto it = headers.begin();it != headers.end(); ++it) {
        response->write(it->first);
        response->write(": ");
        response->writeLine(it->second);
    }
    response->writeLine();
    return true;
}
