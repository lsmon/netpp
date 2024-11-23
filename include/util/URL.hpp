//
// Created by alcacruz on 11/19/24.
//

#ifndef RECOM_URL_HPP
#define RECOM_URL_HPP

#include <iostream>
#include <string>
#include <regex>
#include <optional>

struct URLComponents {
    std::string scheme;
    std::optional<std::string> userInfo;
    std::string host;
    std::optional<int> port;
    std::string path;
    std::optional<std::string> query;
    std::optional<std::string> fragment;
};

class URL {
public:
    static std::optional<URLComponents> parse(const std::string& url);


};


#endif //RECOM_URL_HPP
