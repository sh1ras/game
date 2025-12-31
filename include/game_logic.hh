#include <iostream>
#include <bitset>

namespace srs {
    class score {
        private:
            u_int32_t __vl;
            u_int32_t __max;
            u_int32_t __tolerance;
            void __clanp(u_int32_t& temp);
        public:
            score(u_int32_t max = 0x0F4240, u_int32_t tolerance = 1000) : __vl(0) , __max(max), __tolerance(tolerance) {}
            
            ~score() {}
            
            u_int32_t value() { return __vl; }
            
            score& operator+=(u_int32_t vl) {
                u_int32_t temp = __vl + vl;
                __clanp(temp);
                __vl = temp;
                return *this;
            }
            
            score& init() {
                __vl = 0;
                return *this;
            }
            
    };
}

void srs::score::__clanp(u_int32_t& temp) {
    if (temp < __max) {
        return;   
    } else if (temp - __max <= __tolerance) {
        temp = __max;
        return;
    } else {
        throw std::runtime_error("srs::score::__clanp abnormal deviation detected");
    } return;
}