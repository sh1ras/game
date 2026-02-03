#include "../include/time.hh"

#include <chrono>
#include <stdexcept>

const srs::clock srs::mono_clock;

srs::timer::timer(srs::time_unit t)
    : __start_time(0), __finish_time(0) {}

srs::timer::~timer() {}

void srs::timer::start() {
    __start_time = srs::mono_clock.now_ns();
}

void srs::timer::finish() {
    __finish_time = srs::mono_clock.now_ns();
}

int64_t srs::timer::dt_ns() {
    if (__finish_time < __start_time || __start_time == 0 || __finish_time == 0) {
        throw std::runtime_error("srs::timer::dt abnormal time detected");
    }
    return __finish_time - __start_time;
}

int64_t srs::timer::dt_ms() {
    return dt_ns() / ns_ms;
}

int64_t srs::timer::dt_s() {
    return dt_ns() / ns_s;
}

int64_t srs::timer::start_ns() {
    return __start_time;
}

int64_t srs::timer::finish_ns() {
    return __finish_time;
}

void srs::timer::init() {
    __start_time = 0;
    __finish_time = 0;
}

/* __clock */

srs::clock::clock() : __creation_time_ms(now_ms()) {}

srs::clock::~clock() {}

int64_t srs::clock::now_ns() const {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

int64_t srs::clock::now_ms() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

int64_t srs::clock::now_s() const {
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

int64_t srs::clock::start_ms() const {
    return now_ms() - __creation_time_ms;
}
