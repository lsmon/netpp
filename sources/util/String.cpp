#include "util/String.hpp"

std::vector<std::string> String::tokenize(const std::string str, const std::regex re)
{
    std::vector<std::string> results;
    std::sregex_token_iterator iter(str.begin(), str.end(), re, -1);

    while (iter != std::sregex_token_iterator())
    {
        results.push_back(*iter);
        ++iter;
    }
    return results;
}

bool String::isValidHex(const std::string &str)
{
    return str.compare(0, 2, "0x") == 0 && str.size() > 2 && str.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
}

std::string String::trim(const std::string &str)
{
    std::string trimmedStr;
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    if (start != std::string::npos && end != std::string::npos)
    {
        trimmedStr = str.substr(start, end - start + 1);
    }
    if (trimmedStr.at(0) == '\n')
    {
        trimmedStr.erase(0, 1);
    }
    return trimmedStr;
}

std::string String::getline(std::string str)
{
    std::string line;
    size_t pos = str.find("\n"); // Find the first newline character
    if (pos != std::string::npos)
    {
        line = str.substr(0, pos); // Extract the line
        str = str.substr(pos + 1); // Remove the line from the original string
    }
    else
    {
        line = str; // If no newline character is found, take the whole string
        str = "";   // Clear the original string
    }
    return line;
}

std::string String::replaceUnicodeEscapeSequences(const std::string &input)
{
    std::regex unicodeEscapeSequence(UNICODE_REGEX);
    std::string output;

    for (std::sregex_iterator it(input.begin(), input.end(), unicodeEscapeSequence), end; it != end; ++it)
    {
        std::string match = it->str();
        int codepoint = std::stoi(match.substr(1), 0, 8);
        char ascii = static_cast<char>(codepoint);
        output += ascii;
    }

    return output;
}

bool String::isUnicodeEscapeSequence(const std::string &input)
{
    std::regex unicodeEscapeSequenceRegex(UNICODE_REGEX);
    return std::regex_match(input, unicodeEscapeSequenceRegex);
}
