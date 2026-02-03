#pragma once

#include <cstdint>
#include <chrono>
#include <thread>

namespace srs {

    class clock;
    extern const clock mono_clock; 

    enum class time_unit : uint8_t {
        ns = 0,
        ms = 1,
        s  = 2
    };

    class timer {
        private:
            int64_t __start_time;
            int64_t __finish_time;

            static constexpr int64_t ns_ms = 0xF4240LL;
            static constexpr int64_t ns_s = 0x3B9ACA00LL;

        public:

            timer(time_unit t = time_unit::ms);
            ~timer();

            void set_time_unit(time_unit t);
            
            void start();
            void finish();

            int64_t dt_ns();
            int64_t dt_ms();
            int64_t dt_s(); 

            int64_t start_ns();
            int64_t finish_ns();
            void init();
    };

    class clock {
        private:
            int64_t __creation_time_ms;

        public:
            clock();
            ~clock();

            void init(const int64_t time_ms = 0) {
                __creation_time_ms = time_ms;
            }

            inline std::chrono::milliseconds ms(int64_t s) const {
                return std::chrono::milliseconds(s);
            }
            inline std::chrono::seconds s(int64_t s) const {
                return std::chrono::seconds(s);
            }
            inline std::chrono::minutes min(int64_t s) const {
                return std::chrono::minutes(s);
            }
            inline std::chrono::hours h(int64_t s) const {
                return std::chrono::hours(s);
            }

            int64_t now_ns() const;
            int64_t now_ms() const;
            int64_t now_s() const;
            int64_t start_ms() const;

            template<class Rep, class Period>
            static inline void sleep(const std::chrono::duration<Rep, Period>& t) {
                std::this_thread::sleep_for(t);
            }
    };
}
