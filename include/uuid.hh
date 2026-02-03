#include "srs.hh"
#include <iostream>
#include <random>

namespace srs {
    std::string uuid_v4() {
        static constexpr char hex[] = "0123456789abcdef";
        std::string ret;
        ret.reserve(36);

        for (int i = 0; i < 16; ++i) {
            uint8_t b = static_cast<uint8_t>(srs::random(0, 255));

            if (i == 6) {
                b = (b & 0x0F) | 0x40;
            } if (i == 8) {
                b = (b & 0x3F) | 0x80;
            }

            ret.push_back(hex[b >> 4]);
            ret.push_back(hex[b & 0x0F]);

            if (i == 3 || i == 5 || i == 7 || i == 9) {
                ret.push_back('-');
            }
        }
        return ret;
    }
}