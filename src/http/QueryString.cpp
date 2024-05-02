#include "QueryString.hpp"

void QueryString::parse(const std::string &query)
{
    size_t pos = 0;
    while (pos < query.size()) {
        size_t ampPos = query.find('&', pos);
        std::string pair = query.substr(pos, ampPos - pos);
        size_t equalsPos = pair.find('=');
        if (equalsPos != std::string::npos) {
            std::string key = pair.substr(0, equalsPos);
            std::string value = pair.substr(equalsPos + 1);
            map[key] = value;
        }
        pos = ampPos + 1;
    }
}

std::unordered_map<std::string, std::string> QueryString::getMap()
{
    return std::unordered_map<std::string, std::string>();
}

QueryString &QueryString::getInstance(const std::string& qs)
{
    static QueryString instance; // Guaranteed to be initialized only once
    instance.parse(qs);
    return instance;
}

bool QueryString::isPathQueryString(const std::string &qs)
{
    return qs.find('?') != std::string::npos && qs.find('&') != std::string::npos;;
}
