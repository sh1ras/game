#include "../include/number_16.hh"

inline void srs::number_16::__clanp(int32_t& temp) const {
    if(temp > __max) {
        temp = __max;
    } else if(temp < __min) {
        temp = __min;
    } return;
}

/* public */

srs::number_16::number_16(int16_t num, int16_t max, int16_t min, bool is_allow_overflow)
    : __num(num), __min(min), __max(max), __is_allow_overflow(is_allow_overflow) {}
srs::number_16::number_16(const srs::number_16& me) 
    : __num(me.__num), __min(me.__min), __max(me.__max), __is_allow_overflow(me.__is_allow_overflow) {}

srs::number_16::~number_16() {}

inline srs::number_16 srs::number_16::operator++(int) {
    number_16 tmp = *this;
    if(__is_allow_overflow) 
        __num++;
    else {
        int32_t t = static_cast<int32_t>(__num) + 1;
        if(t > __max) t = __max;
        if(t < __min) t = __min;
        __num = static_cast<int16_t>(t);
    }
    return tmp;
}

inline srs::number_16 srs::number_16::operator--(int) {
    number_16 tmp = *this;
    if(__is_allow_overflow) 
        __num--;
    else {
        int32_t t = static_cast<int32_t>(__num) - 1;
        if(t > __max) t = __max;
        if(t < __min) t = __min;
        __num = static_cast<int16_t>(t);
    }
    return tmp;
}

inline srs::number_16& srs::number_16::operator=(int16_t num) {
    __num = num;
    return *this;
}
inline srs::number_16& srs::number_16::operator+=(const number_16& other) {
    return (*this) += other.to_int16_t();
}

inline srs::number_16& srs::number_16::operator-=(const number_16& other) {
    return (*this) -= other.to_int16_t();
}

inline void srs::number_16::range(int16_t min, int16_t max) {
    __min = min;
    __max = max;
}

inline void srs::number_16::overflow(bool is_allow_overflow) {
    __is_allow_overflow = is_allow_overflow;
    return;
}