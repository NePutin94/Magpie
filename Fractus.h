//
// Created by dimka on 8/24/2022.
//

#ifndef MAGPIE_FRACTUS_H
#define MAGPIE_FRACTUS_H

#include <cassert>
#include <valarray>

namespace Magpie
{
    class Fractus
    {
    private:
        long int denominator;
        long int numerator;

        [[nodiscard]] int gcd(int a, int b) const
        {
            if(a == 0)
                return b;
            return gcd(b % a, a);
        }

    public:
        Fractus(long int numerator, long int denominator) : numerator(numerator), denominator(denominator)
        {

        }

        Fractus operator+(const Fractus& other) const
        {
            auto n1 = numerator * other.denominator;
            auto n2 = other.numerator * denominator;
            long int new_den = denominator * other.denominator, new_num = n1 + n2;
            auto d = gcd(new_den, new_num);
            new_den /= d;
            new_num /= d;
            return Fractus{new_num, new_den};
        }

        Fractus operator-(const Fractus& other) const
        {
            long int new_den, new_num;
            auto n1 = numerator * other.denominator;
            auto n2 = other.numerator * denominator;
            return Fractus{n1 - n2, denominator * other.denominator};
        }

        template<class U>
        void convertTO()
        requires std::is_floating_point_v<U>
        {

            return (U) numerator / denominator;
        }

        template<class U>
        void convertFROM(U number, int cycles = 10, double precision = 5e-4)
        requires std::is_floating_point_v<U>
        {
            int sign = number > 0 ? 1 : -1;
            number = number * sign; //abs(number);
            double new_number, whole_part;
            double decimal_part = number - (int) number;
            int counter = 0;

            std::valarray<double> vec_1{double((int) number), 1}, vec_2{1, 0}, temporary;

            while(decimal_part > precision & counter < cycles)
            {
                new_number = 1 / decimal_part;
                whole_part = (int) new_number;

                temporary = vec_1;
                vec_1 = whole_part * vec_1 + vec_2;
                vec_2 = temporary;

                decimal_part = new_number - whole_part;
                counter += 1;
            }
            numerator = sign * vec_1[0];
            denominator = vec_1[1];
        }
    };
}
#endif //MAGPIE_FRACTUS_H
