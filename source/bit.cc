#include <iostream>
#include <bitset>
#include <cstdint>

namespace srs {
    namespace bit {

        template <typename T>
        constexpr std::size_t __count() {
            static_assert(std::is_integral<T>::value, "T must be integral");
            return sizeof(T) * 8;
        }
        template <typename T>
        constexpr std::size_t count(const T& value) {
            return sizeof(T) * 8;
        }
        template <typename T>
        inline void flip(T& value, std::size_t pos) {
            std::bitset<__count<T>()> bits(value);
            if (pos < bits.size()) 
                bits.flip(pos);
            value = static_cast<T>(bits.to_ullong());
        }
    
        template <typename T>
        inline void set(T& value, std::size_t pos, bool val) {
            std::bitset<__count<T>()> bits(value);
            if (pos < bits.size()) 
                bits.set(pos, val);
            value = static_cast<T>(bits.to_ullong());
        }
    
        template <typename T>
        inline bool get(const T& value, std::size_t pos) {
            std::bitset<__count<T>()> bits(value);
            if (pos < bits.size()) 
                return bits.test(pos);
            return false;
        }
        
        template <typename T>
        inline void set_positive(T& value) {
            if (get(value, __count<T>() - 1)) 
                value = -value;
        }
        template <typename T>
        inline void set_negative(T& value) {
            if (!get(value, __count<T>() - 1)) 
                value = -value;
        }
    
        template <typename T>
        inline void printf(const T& value) {
            std::bitset<__count<T>()> bits(value);
            std::cout << bits << std::endl;
        }
    
    }
}