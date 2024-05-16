#ifndef NETPP_UUID_HPP
#define NETPP_UUID_HPP

#include <vector>
#include <iostream>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>
#include <string>
#include <regex>

class UUID {
private:
    static unsigned char random_uchar();

    static std::string octet(int length);

    static std::string toHexadecimal(unsigned char* buffer, size_t length);

public:
    static std::string random();

    static bool isUUID(std::string str);
};

#endif