#pragma once
#include <corecrt_math.h>
#include <cmath>
#include "internal.h"
#include "loi2proba.h"
#include "p6/p6.h"

enum class lawType {
    uniform,
    poisson,
    SIZE,
};

inline lawType randomLawType()
{
    return static_cast<lawType>((int)p6::random::number((float)lawType::SIZE));
}

inline unsigned int X1(const unsigned int max)
{
    return p6::random::number(max);
}

inline unsigned int X2(const float lambda)
{
    // return exp(-lambda) * (pow(lambda, k) / fact(k)); //Première version un peu nulle
    unsigned int count = 0;
    float        sum   = 0.f;
    do
    {
        sum += (-1.f / lambda) * (std::log(p6::random::number()) / std::log(std::exp(1)));
        // std::cout << sum << std::endl;
        // std::cout << (std::log(p6::random::number()) / std::log(std::exp(1))) << std::endl;
        count++;
    } while (sum < 1.f);

    return count;
}

inline std::string lawTypeToString(lawType type)
{
    switch (type)
    {
    case lawType::uniform:
        return "uniform";
    case lawType::poisson:
        return "poisson";
    default:
        return "unknown";
    }
}
