#include "../include/Fractus.h"

bool Magpie::Fractus::operator==(const Magpie::Fractus& other) const

{
    if(std::numeric_limits<Fractus>::min().numerator == other.numerator
       || std::numeric_limits<Fractus>::min().denominator == other.denominator)
    {
        return other.numerator == numerator && other.denominator == denominator;
    }
    auto n1 = numerator * other.denominator;
    auto n2 = other.numerator * denominator;
    return n1 == n2;
}