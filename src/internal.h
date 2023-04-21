#pragma once

unsigned int fact(unsigned int k)
{
    unsigned int result = 1;
    for (unsigned int i = 1; i <= k; i++)
    {
        result *= i;
    }
    return result;
}