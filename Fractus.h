
#ifndef MAGPIE_FRACTUS_H
#define MAGPIE_FRACTUS_H

#include <cassert>
#include <valarray>
#include <string>
#include <fmt/format.h>

namespace Magpie
{
    class Fractus
    {
    private:
        long int denominator = 1;
        long int numerator = 0;

        [[nodiscard]] int gcd(int a, int b) const
        {
            if(a == 0)
                return b;
            return gcd(b % a, a);
        }

    public:
        Fractus() = default;

        Fractus(long int numerator, long int denominator) : numerator(numerator), denominator(denominator)
        {}

        Fractus(const std::string& str)
        {
            convert(str);
        }

        template<class T>
        requires std::is_arithmetic_v<T>
        explicit Fractus(const T& v)
        {
            this->numerator = v;
            this->denominator = 1;
        }

        auto getNumerator() const
        {
            return numerator;
        }

        void setNumerator(long int numerator)
        {
            this->numerator = numerator;
        }

        Fractus operator+(const Fractus& other) const
        {
            auto n1 = numerator * other.denominator;
            auto n2 = other.numerator * denominator;
//            auto n1 = numerator * other.denominator;
//            auto n2 = other.numerator * denominator;
//            long int new_den = denominator * other.denominator, new_num = n1 + n2;
//            auto d = gcd(new_den, new_num);
//            new_den /= d;
//            new_num /= d;
            return Fractus{n1 + n2, denominator * other.denominator};
        }

        Fractus operator-=(const Fractus& other)
        {
            *this = *this - other;
            return *this;
        }

        Fractus operator+=(const Fractus& other)
        {
            *this = *this + other;
            return *this;
        }

        Fractus operator/=(const Fractus& other)
        {
            *this = *this / other;
            return *this;
        }

        Fractus operator*=(const Fractus& other)
        {
            *this = *this * other;
            return *this;
        }

        Fractus operator-(const Fractus& other) const
        {
            auto n1 = numerator * other.denominator;
            auto n2 = other.numerator * denominator;
            return Fractus{n1 - n2, denominator * other.denominator};
        }

        Fractus operator-() const
        {
            return Fractus{-numerator, denominator};
        }

        Fractus operator/(const Fractus& other) const
        {
            auto n1 = numerator * other.denominator;
            auto n2 = denominator * other.numerator;
            return Fractus{n1, n2};
        }

        Fractus operator*(const Fractus& other) const
        {
            auto n1 = numerator * other.numerator;
            auto n2 = other.denominator * denominator;
            return Fractus{n1, n2};
        }

        template<class T>
        requires std::is_arithmetic_v<T>
        bool operator==(const T& val) const
        {
            return (T) *this == val;
        }

        template<class T>
        requires std::is_arithmetic_v<T>
        bool operator!=(const T& val) const
        {
            return (T) *this != val;
        }

        bool operator==(const Fractus& other) const
        {
            auto n1 = numerator * other.denominator;
            auto n2 = other.numerator * denominator;
            return n1 == n2;
        }

        bool operator!=(const Fractus& other) const
        {
            auto n1 = numerator * other.denominator;
            auto n2 = other.numerator * denominator;
            return n1 != n2;
        }

        bool operator<(const Fractus& other) const
        {
            auto n1 = numerator * other.denominator;
            auto n2 = other.numerator * denominator;
            return n1 < n2;
        }

        bool operator<=(const Fractus& other) const
        {
            auto n1 = numerator * other.denominator;
            auto n2 = other.numerator * denominator;
            return n1 <= n2;
        }

        bool operator>(const Fractus& other) const
        {
            auto n1 = numerator * other.denominator;
            auto n2 = other.numerator * denominator;
            return n1 > n2;
        }

        bool operator>=(const Fractus& other) const
        {
            auto n1 = numerator * other.denominator;
            auto n2 = other.numerator * denominator;
            return n1 >= n2;
        }

        explicit operator int() const
        {
            return numerator / denominator;
        }

        template<class T>
        requires std::is_floating_point_v<T>
        explicit operator T() const
        {
            return convertTO<T>();
        }

        explicit operator std::string() const
        {
            return toString();
        }

        template<class T>
        requires std::is_arithmetic_v<T>
        Fractus& operator=(const T& other)
        {
            this->denominator = 1;
            this->numerator = other;
            return *this;
        }

        template<class T>
        requires std::is_arithmetic_v<T>
        Fractus operator-(const T& other) const
        {
            return *this - Fractus(other);
        }

        template<class T>
        requires std::is_arithmetic_v<T>
        Fractus operator+(const T& other) const
        {
            return *this + Fractus(other);
        }

        template<class U>
        auto convertTO() const
        requires std::is_floating_point_v<U>
        {
            return (U) numerator / denominator;
        }

        std::string toString() const
        {
            return fmt::format("{0}/{0}", numerator, denominator);
        }

        void convert(const std::string& number)
        {
            auto slash_pos = number.find_first_of('/');

            auto nume_str = number.substr(0, slash_pos);
            auto denu_str = number.substr(slash_pos + 1, number.size() - slash_pos);

            numerator = std::stol(nume_str);
            denominator = std::stol(denu_str);
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

        Fractus abs() const
        {
            return Fractus{std::abs(numerator), std::abs(denominator)};
        }
    };

    template<class T>
    auto abs(const T& t)
    {
        if constexpr(std::is_same_v<T, Fractus>)
        {
            return t.abs();
        } else
            return std::abs(t);
    }
}

namespace std
{
    template<>
    class numeric_limits<Magpie::Fractus>
    {
    public:
        static Magpie::Fractus min()
        { return {std::numeric_limits<long int>::min(), std::numeric_limits<long int>::min()}; };

        static Magpie::Fractus max()
        { return {std::numeric_limits<long int>::max(), std::numeric_limits<long int>::max()}; };


        static constexpr bool is_iec559 = true;
    };
}
#endif //MAGPIE_FRACTUS_H
