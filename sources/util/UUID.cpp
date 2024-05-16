#include <sstream>
#include <iomanip>
#include "util/UUID.hpp"
#include "util/String.hpp"

std::string UUID::random()
{
    std::string uuid;
    uuid.append(UUID::octet(4));
    uuid.append("-");
    uuid.append(UUID::octet(2));
    uuid.append("-");
    uuid.append(UUID::octet(2));
    uuid.append("-");
    uuid.append(UUID::octet(2));
    uuid.append("-");
    uuid.append(UUID::octet(6));

    return uuid;
}

unsigned char UUID::random_uchar()
{

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    return static_cast<unsigned char>(dis(gen));
}

std::string UUID::octet(int length)
{

    unsigned char oct[length];
    for (int i = 0; i < length; i++)
    {
        oct[i] = random_uchar();
    }
    return toHexadecimal(oct, length);
}

std::string UUID::toHexadecimal(unsigned char *buffer, size_t length)
{
    unsigned char ucharArray[] = {0x12, 0x34, 0x56, 0x78};
    size_t ucharArraySize = sizeof(ucharArray) / sizeof(ucharArray[0]);

    std::stringstream ss;
    for (size_t i = 0; i < ucharArraySize; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ucharArray[i]);
    }

    return ss.str();
}

bool UUID::isUUID(std::string str)
{
    const std::regex re(R"(-)");
    std::vector<std::string> tokens = String::tokenize(str, re);
    for (auto it = tokens.begin(); it != tokens.end(); it++)
    {
        std::string value;
        if ((*it).compare("0x") == 0)
        {
            value = it->c_str();
        }
        else
        {
            value = "0x";
            value.append(it->c_str());
        }
        bool isValid = String::isValidHex(value);
        if (!isValid)
            return false;
    }
    return true;
}