#ifndef NETPP_STRING_HPP
#define NETPP_STRING_HPP

#include <string>
#include <regex>
#include <vector>

#define UNICODE_REGEX R"((\n[0-9a-fA-F]{1,4}))"
class String
{
public:
    String() = default;
    ~String() = default;

    /**
     * @brief Tokenize the given vector according to the regex and remove the empty tokens.
     * @param str
     * @param re
     * @return std::vector<std::string>
     */
    static std::vector<std::string> tokenize(const std::string str, const std::regex re);

    /**
     * validate if string matches the hexadecimal format
     * @param str - the string to be validate.
     * @return - boolean value `true` if the string format is hexadecimal, `false` otherwise.
     */
    static bool isValidHex(const std::string &str);

    static std::string trim(const std::string& str);

    static std::string getline(std::string str);

    static std::string replaceUnicodeEscapeSequences(const std::string &input);
            
    static bool isUnicodeEscapeSequence(const std::string &input);

};

#endif