//
// Created by alcacruz on 11/21/24.
//

#ifndef NETPP_PATH_HPP
#define NETPP_PATH_HPP

#include <string>
#include <map>
#include <vector>

#include <string>
#include <map>
#include <vector>
#include <regex>
#include <iostream>

class Path {
private:
    std::string pattern;
    std::regex regexPattern;
    std::vector<std::string> paramKeys;
    std::map<std::string, std::string> pathParamMap;

    void compilePattern(const std::string &pathPattern);

public:
    Path() = default;

    void setPathPattern(const std::string &pathPattern);

    explicit Path(const std::string &pathPattern);

    bool match(const std::string &path);

    const std::map<std::string, std::string> &getPathParamMap() const;
};

#endif //NETPP_PATH_HPP
