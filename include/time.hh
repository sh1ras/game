#pragma once 

#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <cstdlib>
#include <cstdio>
#include <string>

namespace srs {
    extern srs::__clock clock;
    enum class time_unit : u_int8_t {
        ns = 0,
        ms = 1,
        s = 2
    };
    class timer {
        private:
            int64_t __creation_time_ms;
            int64_t __start_time;
            int64_t __finish_time;
            srs::time_unit __time_unit;
        public:
            timer(srs::time_unit t = srs::time_unit::ms);
            ~timer();

            void set_time_unit(srs::time_unit t);
            void start();
            void finish();

            int64_t dt();
            int64_t dt_a();
            int64_t start_time();
            int64_t finish_time();
            void init();
    };

    class __clock {
        private:
            int64_t __creation_time_ms;
        public:
            __clock() : __creation_time_ms(now_ms()) {}
            ~__clock() {}
            inline std::chrono::milliseconds ms(const int64_t& s);
            inline std::chrono::seconds s(const int64_t& s);
            inline std::chrono::minutes min(const int64_t& s);
            inline std::chrono::hours h(const int64_t& s);
            inline int64_t now_ns();
            inline int64_t now_ms();
            inline int64_t start_ms();
            inline int64_t now_s();
            
            template<class Rep, class Period>
            inline void this_sleep(const std::chrono::duration<Rep, Period>& t) {
                std::this_thread::sleep_for(t);
                return;
            }
    };
}