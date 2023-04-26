#pragma once

#include <p6/p6.h>
#include <codecvt>

unsigned int fact(unsigned int k)
{
    unsigned int result = 1;
    for (unsigned int i = 1; i <= k; i++)
    {
        result *= i;
    }
    return result;
}

inline std::u16string to_u16string(std::string str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
    return conv.from_bytes(str);
}

glm::vec2 indexToVector(unsigned int index)
{
    if (index < 10)
    {
        return glm::vec2(1.f, 0.f);
    }
    if (index < 19)
    {
        return glm::vec2(0.f, 1.f);
    }
    if (index < 28)
    {
        return glm::vec2(-1.f, 0.f);
    }
    if (index < 36)
    {
        return glm::vec2(0.f, -1.f);
    }
    if (index < 44)
    {
        return glm::vec2(1.f, 0.f);
    }
    if (index < 51)
    {
        return glm::vec2(0.f, 1.f);
    }
    if (index < 58)
    {
        return glm::vec2(-1.f, 0.f);
    }
    if (index < 64)
    {
        return glm::vec2(0.f, -1.f);
    }
    if (index < 70)
    {
        return glm::vec2(1.f, 0.f);
    }
    if (index < 75)
    {
        return glm::vec2(0.f, 1.f);
    }
    if (index < 78)
    {
        return glm::vec2(-1.f, 0.f);
    }
}

template<typename T>
inline constexpr int sign(T val)
{
    return (T{0} <= val) - (val < T{0});
}