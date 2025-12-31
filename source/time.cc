#include "../include/time.hh"

srs::__clock srs::clock;

srs::timer::timer(srs::time_unit t) : __start_time(0), __finish_time(0), __time_unit(t) {}

srs::timer::~timer() {}

void srs::timer::set_time_unit(srs::time_unit t) {
    __time_unit = t;
}

void srs::timer::start() {
    if(__time_unit == srs::time_unit::ns) {
        __start_time = srs::clock.now_ns();
    } else if (__time_unit == srs::time_unit::ms) {
        __start_time = srs::clock.now_ms();
    } else if (__time_unit == srs::time_unit::s) {
        __start_time = srs::clock.now_s();
    }
}

void srs::timer::finish() {
    if(__time_unit == srs::time_unit::ns) {
        __finish_time = srs::clock.now_ns();
    } else if (__time_unit == srs::time_unit::ms) {
        __finish_time = srs::clock.now_ms();
    } else if (__time_unit == srs::time_unit::s) {
        __finish_time = srs::clock.now_s();
    }
}

int64_t srs::timer::dt() {
    if(__finish_time < __start_time || __start_time == 0 || __finish_time == 0) {
        throw std::runtime_error("srs::timer::dt abnormal time detected");
    }
    return __finish_time - __start_time;
}

int64_t srs::timer::dt_a() {
    if(__finish_time < __start_time || __start_time == 0 || __finish_time == 0) {
        throw std::runtime_error("srs::timer::dt_a abnormal time detected");
    }
    return __finish_time - __start_time - 30;
}

int64_t srs::timer::start_time() {
    return __start_time;
}

int64_t srs::timer::finish_time() {
    return __finish_time;
}

void srs::timer::init() {
    __start_time = 0;
    __finish_time = 0;
}

std::chrono::milliseconds srs::__clock::ms(const int64_t& s) {
    return std::chrono::milliseconds(s);
}

std::chrono::seconds srs::__clock::s(const int64_t& s) {
    return std::chrono::seconds(s);
}

std::chrono::minutes srs::__clock::min(const int64_t& s) {
    return std::chrono::minutes(s);
}

std::chrono::hours srs::__clock::h(const int64_t& s) {
    return std::chrono::hours(s);
}

int64_t srs::__clock::now_ns() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

int64_t srs::__clock::now_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

int64_t srs::__clock::now_s() {
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

int64_t srs::__clock::start_ms() {
    return __creation_time_ms - now_ms();
}
