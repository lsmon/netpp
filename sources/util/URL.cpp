//
// Created by alcacruz on 11/19/24.
//

#include "util/URL.hpp"

std::optional<URLComponents> URL::parse(const std::string &url) {
    const std::regex urlRegex(R"(^(\w+):\/\/(?:([\w%:.!~*'()-]+)@)?([\w.-]+)(?::(\d+))?(\/[^\?#]*)?(?:\?([^#]*))?(?:#(.*))?$)");
    std::smatch match;

    if (std::regex_match(url, match, urlRegex)) {
        URLComponents components;
        components.scheme = match[1];
        if (match[2].matched) components.userInfo = match[2];
        components.host = match[3];
        if (match[4].matched) components.port = std::stoi(match[4]);
        if (match[5].matched) components.path = match[5];
        if (match[6].matched) components.query = match[6];
        if (match[7].matched) components.fragment = match[7];

        return components;
    }

    return std::nullopt;
}
