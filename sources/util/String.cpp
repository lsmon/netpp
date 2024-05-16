#include "util/String.hpp"

std::vector<std::string> String::tokenize(const std::string str, const std::regex re)
{
    std::vector<std::string> results;
    std::sregex_token_iterator iter(str.begin(), str.end(), re, -1);

    while (iter != std::sregex_token_iterator()){
        results.push_back(*iter);
        ++iter;
    }
    return results;
}

bool String::isValidHex(const std::string &str) {
    return str.compare(0, 2, "0x") == 0
           && str.size() > 2
           && str.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
}