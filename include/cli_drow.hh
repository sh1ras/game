#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <ranges> 
#include "../source/debug.cc"
#include "./number_16.hh"
#include "./character.hh"

namespace srs {
    
/*     struct character_data {
        character_data() = default;
        ~character_data() = default;
        std::string name = "Unknown";
        srs::number_16 hp = 0;
        srs::number_16 mp = 0;
        srs::number_16 atk = 0;
    }; */
    class drow {
        private:
            inline void __status_drow(
                const std::vector<srs::character>& character_data, 
                const std::vector<u_int16_t>& name_len,
                const std::string& label,
                auto get_value,
                const int& n = 0
            );
        public:
            drow() {}
            ~drow() {}
            void vector_dorw(const std::vector<std::string>& drow_data);
            void character_drow(const std::vector<srs::character>& character_data, const int& n = 0);
            void action_drow(const std::vector<std::string>& action);
    };
    extern srs::drow sdorw;
}
