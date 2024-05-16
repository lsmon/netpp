#ifndef NETPP_QUERY_STRING
#define NETPP_QUERY_STRING

#include <string>
#include <unordered_map>
#include <sstream>

class QueryString
{
private:
    QueryString() = default;

    void parse(const std::string& queryString);

    std::unordered_map<std::string, std::string> map;
public:
    std::unordered_map<std::string, std::string> getMap();
    
    static QueryString& getInstance(const std::string& qs);

    static bool isPathQueryString(const std::string& qs);
};

#endif