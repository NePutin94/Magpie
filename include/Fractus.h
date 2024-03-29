
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
        enum SIGN
        {
            MINUS,
            PLUS
        };
        SIGN _sign = PLUS;

        [[nodiscard]] int gcd(long int a, long int b) const
        {
            if(a == 0)
                return b;
            return gcd(b % a, a);
        }

        void setSign()
        {
            _sign = (numerator < 0 && denominator > 0 || numerator > 0 && denominator < 0) ? MINUS : PLUS;
        }

        void applySign()
        {
            setSign();
            switch(_sign)
            {
                case MINUS:
                    this->denominator = std::abs(this->denominator);
                    this->numerator = -std::abs(this->numerator);
                    break;
                case PLUS:
                    this->denominator = std::abs(this->denominator);
                    this->numerator = std::abs(this->numerator);
                    break;
            }
        }

        void simplify()
        {

            if(numerator == 0 || denominator == 0)
            {
                numerator = 0;
                denominator = 1;
                return;
            }

            long int a, b, c;
            a = numerator;
            b = denominator;

            while(a % b != 0)
            {
                c = a % b;
                a = b;
                b = c;
            }
            numerator /= b;
            denominator /= b;

            applySign();
        }

        void normalize()
        {
            if(numerator == 0 || denominator == 0)
            {
                numerator = 0;
                denominator = 1;
            }
            int n = gcd(numerator, denominator);
            numerator = numerator / n;
            denominator = denominator / n;
            applySign();
        }

    public:
        Fractus() = default;

        Fractus(long int numerator, long int denominator, bool makeSimple = false) : numerator(numerator), denominator(denominator)
        {
            if(makeSimple)
                simplify();
            else
                applySign();
        }

        Fractus(const std::string& str)
        {
            convert(str);
        }

        template<class T>
        requires std::is_floating_point_v<T>
        Fractus(T n)
        {
            convertFROM(n);
        }

        template<class T>
        requires std::is_integral_v<T>
        Fractus(T v)
        {
            this->numerator = v;
            this->denominator = 1;
            applySign();
        }

        auto getNumerator() const
        {
            return numerator;
        }

        void setNumerator(long int numerator)
        {
            this->numerator = numerator;
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
            return Fractus{n1 - n2, denominator * other.denominator, true};
        }

        Fractus operator+(const Fractus& other) const
        {
            auto n1 = numerator * other.denominator;
            auto n2 = other.numerator * denominator;
            return Fractus{n1 + n2, denominator * other.denominator, true};
        }

        Fractus operator-() const
        {
            return Fractus{-numerator, std::abs(denominator)};
        }

        Fractus operator/(const Fractus& other) const
        {
            auto n1 = numerator * other.denominator;
            auto n2 = denominator * other.numerator;
            return Fractus{n1, n2, true};
        }

        Fractus operator*(const Fractus& other) const
        {
            auto n1 = numerator * other.numerator;
            auto n2 = other.denominator * denominator;
            return Fractus{n1, n2, true};
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
            return !(*this == val);
        }

        bool operator==(const Fractus& other) const;

        bool operator!=(const Fractus& other) const
        {
            return !(*this == other);
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
//
//        template<class T>
//        requires std::is_integral_v<T>
//        Fractus& operator=(const T& other)
//        {
//            this->denominator = 1;
//            this->numerator = other;
//            return *this;
//        }
//
//        template<class T>
//        requires std::is_floating_point_v<T>
//        Fractus& operator=(const T& other)
//        {
//            convertFROM<T>(other);
//            return *this;
//        }

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
        U convertTO() const
        requires std::is_floating_point_v<U>
        {
            return (U) numerator / denominator;
        }

        std::string toString() const
        {
            return fmt::format("{}/{}", numerator, denominator);
        }

        void convert(const std::string& number)
        {
            auto slash_pos = number.find_first_of('/');

            if(slash_pos == std::string::npos)
            {
                numerator = std::stol(number);
                denominator = 1;
            } else
            {
                auto nume_str = number.substr(0, slash_pos);
                auto denu_str = number.substr(slash_pos + 1, number.size() - slash_pos);
                numerator = std::stol(nume_str);
                denominator = std::stol(denu_str);
            }
            applySign();
        }

        static Fractus convertString(const std::string& number)
        {
            Fractus f;
            auto slash_pos = number.find_first_of('/');

            if(slash_pos == std::string::npos)
            {
                f.numerator = std::stol(number);
                f.denominator = 1;
            } else
            {
                auto nume_str = number.substr(0, slash_pos);
                auto denu_str = number.substr(slash_pos + 1, number.size() - slash_pos);
                f.numerator = std::stol(nume_str);
                f.denominator = std::stol(denu_str);
            }
            f.applySign();
            return f;
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
            applySign();
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

namespace fmt
{

#include <fmt/core.h>

    template<>
    struct formatter<Magpie::Fractus> :
            formatter<std::string>
    {

        template<typename FormatContext>
        auto format(const Magpie::Fractus& number, FormatContext& ctx)
        {
            return formatter<std::string>::format(number.toString(), ctx);
            //  return fmt::format_to(ctx.out(), "{:.2f}", number.toString());
        }
    };
}

#include <Vec2.h>

namespace glm
{
    inline auto sqrt(const palka::Vec3<Magpie::Fractus>& vec)
    {
        palka::Vec3<double> vec_d = {vec.x.convertTO<double>(), vec.y.convertTO<double>(), vec.z.convertTO<double>()};
        auto res = glm::sqrt(vec_d);
        return palka::Vec3<Magpie::Fractus>(Magpie::Fractus(res.x), Magpie::Fractus(res.y), Magpie::Fractus(res.z));
    }

    inline auto sqrt(const glm::vec<3, Magpie::Fractus, glm::defaultp>& vec)
    {
        glm::vec<3, double, glm::defaultp> vec_d = {vec.x.convertTO<double>(), vec.y.convertTO<double>(), vec.z.convertTO<double>()};
        auto res = glm::sqrt(vec_d);
        return palka::Vec3<Magpie::Fractus>(Magpie::Fractus(res.x), Magpie::Fractus(res.y), Magpie::Fractus(res.z));
    }

    inline auto sqrt(const palka::Vec2<Magpie::Fractus>& vec)
    {
        palka::Vec2<double> vec_d = {vec.x.convertTO<double>(), vec.y.convertTO<double>()};
        auto res = glm::sqrt(vec_d);
        return palka::Vec2<Magpie::Fractus>(Magpie::Fractus(res.x), Magpie::Fractus(res.y));
    }

    inline auto sqrt(const glm::vec<2, Magpie::Fractus, glm::defaultp>& vec)
    {
        glm::vec<2, double, glm::defaultp> vec_d = {vec.x.convertTO<double>(), vec.y.convertTO<double>()};
        auto res = glm::sqrt(vec_d);
        return palka::Vec2<Magpie::Fractus>(Magpie::Fractus(res.x), Magpie::Fractus(res.y));
    }

    inline auto length(const palka::Vec2<Magpie::Fractus>& vec)
    {
        glm::vec<2, double, glm::defaultp> vec_d = {vec.x.convertTO<double>(), vec.y.convertTO<double>()};
        auto res = glm::length(vec_d);
        return Magpie::Fractus(res);
    }

    inline auto length(const glm::vec<2, Magpie::Fractus, glm::defaultp>& vec)
    {
        glm::vec<2, double, glm::defaultp> vec_d = {vec.x.convertTO<double>(), vec.y.convertTO<double>()};
        auto res = glm::length(vec_d);
        return Magpie::Fractus(res);
    }

    inline auto length(const palka::Vec3<Magpie::Fractus>& vec)
    {
        glm::vec<3, double, glm::defaultp> vec_d = {vec.x.convertTO<double>(), vec.y.convertTO<double>(), vec.z.convertTO<double>()};
        auto res = glm::length(vec_d);
        return Magpie::Fractus(res);
    }

    inline auto length(const glm::vec<3, Magpie::Fractus, glm::defaultp>& vec)
    {
        glm::vec<3, double, glm::defaultp> vec_d = {vec.x.convertTO<double>(), vec.y.convertTO<double>(), vec.z.convertTO<double>()};
        auto res = glm::length(vec_d);
        return Magpie::Fractus(res);
    }

    inline auto normalize(const palka::Vec2<Magpie::Fractus>& vec)
    {
        glm::vec<2, double, glm::defaultp> vec_d = {vec.x.convertTO<double>(), vec.y.convertTO<double>()};
        auto res = glm::normalize(vec_d);
        return palka::Vec2<Magpie::Fractus>(Magpie::Fractus(res.x), Magpie::Fractus(res.y));
    }

    inline auto normalize(const glm::vec<2, Magpie::Fractus, glm::defaultp>& vec)
    {
        glm::vec<2, double, glm::defaultp> vec_d = {vec.x.convertTO<double>(), vec.y.convertTO<double>()};
        auto res = glm::normalize(vec_d);
        return palka::Vec2<Magpie::Fractus>(Magpie::Fractus(res.x), Magpie::Fractus(res.y));
    }

    inline auto normalize(const palka::Vec3<Magpie::Fractus>& vec)
    {
        glm::vec<3, double, glm::defaultp> vec_d = {vec.x.convertTO<double>(), vec.y.convertTO<double>(), vec.z.convertTO<double>()};
        auto res = glm::normalize(vec_d);
        return palka::Vec3<Magpie::Fractus>(Magpie::Fractus(res.x), Magpie::Fractus(res.y), Magpie::Fractus(res.z));
    }

    inline auto normalize(const glm::vec<3, Magpie::Fractus, glm::defaultp>& vec)
    {
        glm::vec<3, double, glm::defaultp> vec_d = {vec.x.convertTO<double>(), vec.y.convertTO<double>(), vec.z.convertTO<double>()};
        auto res = glm::normalize(vec_d);
        return palka::Vec3<Magpie::Fractus>(Magpie::Fractus(res.x), Magpie::Fractus(res.y), Magpie::Fractus(res.z));
    }
}
namespace std
{
    inline long double sqrt(Magpie::Fractus val) noexcept
    {
        return std::sqrt(val.convertTO<double>());
    }

    template<>
    class numeric_limits<Magpie::Fractus>
    {
    public:
        static Magpie::Fractus min()
        { return {std::numeric_limits<long int>::min(), 1}; };

        static Magpie::Fractus max()
        { return {std::numeric_limits<long int>::max(), 1}; };


        static constexpr bool is_iec559 = true;
    };
}
#endif //MAGPIE_FRACTUS_H
