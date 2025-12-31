#pragma once

#include <iostream>
#include <cstdint>
#include <type_traits>

#ifndef NUMBER16_MAX
#   define NUMBER16_MAX INT16_MAX
#endif
#ifndef NUMBER16_MIN
#   define NUMBER16_MIN INT16_MIN
#endif

namespace srs {

    class number_16 {
        private:
            int16_t __num;
            int16_t __min;
            int16_t __max;
            bool    __is_allow_overflow;

            inline void __clanp(int32_t& temp) const;

            template<typename T, typename Op>
            int16_t __calc(T vl, Op op) const {
                if (__is_allow_overflow) {
                    return static_cast<int16_t>(op(__num, vl));
                }

                int32_t temp = op(static_cast<int32_t>(__num), vl);
                __clanp(temp);
                return static_cast<int16_t>(temp);
            }
        public:
            number_16(
                int16_t num = 0,
                int16_t max = INT16_MAX,
                int16_t min = INT16_MIN,
                bool is_allow_overflow = true
            );

            number_16(const srs::number_16& me);
            ~number_16();

            template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
            int16_t operator+(T vl) const {
                return __calc(vl, [](auto a, auto b) { return a + b; });
            }

            template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
            int16_t operator-(T vl) const{
                return __calc(vl, [](auto a, auto b) { return a - b; });
            }

            template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
            int16_t operator*(T vl) const {
               return __calc(vl, [](auto a, auto b) { return a * b; });
            }

            template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
            int16_t operator/(T vl) const {
                return __calc(vl, [](auto a, auto b) { return a / b; });
            }

            number_16 operator+(const number_16& other) const {
                number_16 tmp = *this;
                tmp += other;
                return tmp;
            }

            number_16 operator-(const number_16& other) const {
                number_16 tmp = *this;
                tmp -= other;
                return tmp;
            }

            number_16 operator*(const number_16& other) const {
                number_16 tmp = *this;
                tmp *= other.to_int16_t();
                return tmp;
            }

            number_16 operator/(const number_16& other) const {
                number_16 tmp = *this;
                tmp /= other.to_int16_t();
                return tmp;
            }

            number_16 operator++(int);
            number_16 operator--(int);
            number_16& operator=(int16_t num);
            number_16& operator+=(const number_16& other);
            number_16& operator-=(const number_16& other);

            template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
            number_16& operator+=(T vl) {
                __num = __calc(vl, [](auto a, auto b) { return a + b; });
                return *this;
            }

            template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
            number_16& operator-=(T vl) {
                __num = __calc(vl, [](auto a, auto b) { return a - b; });
                return *this;
            }

            template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
            number_16& operator*=(T vl) {
                __num = __calc(vl, [](auto a, auto b) { return a * b; });
                return *this;
            }

            template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
            number_16& operator/=(T vl) {
                __num = __calc(vl, [](auto a, auto b) { return a / b; });
                return *this;
            }

            template<typename T>
            bool operator==(const T& value) const {
                return __num == value;
            }

            template<typename T>
            bool operator<(const T& value) const {
                return __num < value;
            }

            template<typename T>
            bool operator>(const T& value) const {
                return __num > value;
            }

            template<typename T>
            bool operator<=(const T& value) const {
                return __num <= value;
            }

            template<typename T>
            bool operator>=(const T& value) const {
                return __num >= value;
            }

            int16_t& to_int16_t() {
                return __num;
            }

            const int16_t& to_int16_t() const {
                return __num;
            }

            std::string to_string() const {
                return std::to_string(__num);
            }

            explicit operator int16_t() const {
                return __num;
            }

            void range(int16_t min, int16_t max);
            void overflow(bool is_allow_overflow);
    };

    inline std::ostream& operator<<(std::ostream& os, const number_16& n) {
        return os << n.to_int16_t();
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    inline number_16 operator+(const T& lhs, const number_16& rhs) {
        number_16 tmp(rhs);
        tmp += lhs;
        return tmp;
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    inline number_16 operator-(T lhs, const number_16& rhs) {
        number_16 tmp(static_cast<int16_t>(lhs));
        tmp -= rhs;
        return tmp;
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    inline number_16 operator*(T lhs, const number_16& rhs) {
        number_16 tmp(rhs);
        tmp *= lhs;
        return tmp;
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    inline number_16 operator/(T lhs, const number_16& rhs) {
        number_16 tmp(static_cast<int16_t>(lhs));
        tmp /= rhs.to_int16_t();
        return tmp;
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    inline bool operator==(T lhs, const number_16& rhs) {
        return lhs == rhs.to_int16_t();
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    inline bool operator!=(T lhs, const number_16& rhs) {
        return lhs != rhs.to_int16_t();
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    inline bool operator<(T lhs, const number_16& rhs) {
        return lhs < rhs.to_int16_t();
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    inline bool operator<=(T lhs, const number_16& rhs) {
        return lhs <= rhs.to_int16_t();
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    inline bool operator>(T lhs, const number_16& rhs) {
        return lhs > rhs.to_int16_t();
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    inline bool operator>=(T lhs, const number_16& rhs) {
        return lhs >= rhs.to_int16_t();
    }

}
