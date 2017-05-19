#include "util.hpp"

std::regex Util::regexNumber("^[-+]?[0-9]+(?:\\.[0-9]+)?$");
std::regex Util::regexInteger("^\\d+$");

bool Util::isNumber(std::string str) {
    return std::regex_match(str, regexNumber);
}

bool Util::isInteger(std::string str) {
    return std::regex_match(str, regexInteger);
}
