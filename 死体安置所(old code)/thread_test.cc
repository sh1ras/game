#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <numeric>
#include <chrono>
#include <random>
#include <mutex>
#include <string>
#include <cstdlib>
#include <iomanip>

using ms_t = long long;

static std::vector<int> generate_data(size_t N, unsigned seed = 12345) {
    std::vector<int> v;
    v.reserve(N);
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 10);
    for (size_t i = 0; i < N; ++i) v.push_back(dist(rng));
    return v;
}

static std::pair<long long, ms_t> single_thread_sum(const std::vector<int>& v) {
    using namespace std::chrono;
    auto t0 = high_resolution_clock::now();
    long long s = std::accumulate(v.begin(), v.end(), 0LL);
    auto t1 = high_resolution_clock::now();
    return {s, duration_cast<milliseconds>(t1 - t0).count()};
}

static std::pair<long long, ms_t> atomic_parallel_sum(const std::vector<int>& v, unsigned num_threads) {
    using namespace std::chrono;
    std::atomic<long long> atomic_sum{0};
    size_t N = v.size();
    size_t chunk = N / num_threads;
    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    auto t0 = high_resolution_clock::now();
    for (unsigned t = 0; t < num_threads; ++t) {
        size_t begin = t * chunk;
        size_t end = (t + 1 == num_threads) ? N : begin + chunk;
        threads.emplace_back([&v, begin, end, &atomic_sum]() {
            long long local = 0;
            for (size_t i = begin; i < end; ++i) local += v[i];
            atomic_sum.fetch_add(local, std::memory_order_relaxed);
        });
    }
    for (auto &th : threads) th.join();
    auto t1 = high_resolution_clock::now();
    return {atomic_sum.load(), duration_cast<milliseconds>(t1 - t0).count()};
}

static std::pair<long long, ms_t> partial_reduce_parallel_sum(const std::vector<int>& v, unsigned num_threads) {
    using namespace std::chrono;
    size_t N = v.size();
    size_t chunk = N / num_threads;
    std::vector<long long> partials(num_threads, 0);
    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    auto t0 = high_resolution_clock::now();
    for (unsigned t = 0; t < num_threads; ++t) {
        size_t begin = t * chunk;
        size_t end = (t + 1 == num_threads) ? N : begin + chunk;
        threads.emplace_back([&v, begin, end, &partials, t]() {
            long long local = 0;
            for (size_t i = begin; i < end; ++i) local += v[i];
            partials[t] = local;
        });
    }
    for (auto &th : threads) th.join();
    long long reduced = 0;
    for (auto p : partials) reduced += p;
    auto t1 = high_resolution_clock::now();
    return {reduced, duration_cast<milliseconds>(t1 - t0).count()};
}

static std::pair<long long, ms_t> mutex_parallel_sum(const std::vector<int>& v, unsigned num_threads) {
    using namespace std::chrono;
    long long shared = 0;
    std::mutex m;
    size_t N = v.size();
    size_t chunk = N / num_threads;
    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    auto t0 = high_resolution_clock::now();
    for (unsigned t = 0; t < num_threads; ++t) {
        size_t begin = t * chunk;
        size_t end = (t + 1 == num_threads) ? N : begin + chunk;
        threads.emplace_back([&v, begin, end, &shared, &m]() {
            long long local = 0;
            for (size_t i = begin; i < end; ++i) local += v[i];
            std::lock_guard<std::mutex> lk(m);
            shared += local;
        });
    }
    for (auto &th : threads) th.join();
    auto t1 = high_resolution_clock::now();
    return {shared, duration_cast<milliseconds>(t1 - t0).count()};
}

static void print_result(const std::string& label, const std::pair<long long, ms_t>& res) {
    std::cout << label << ": sum=" << res.first << " time=" << res.second << "ms\n";
}

static void usage(const char* prog) {
    std::cout << "Usage: " << prog << " [N] [num_threads]\n";
    std::cout << "  N: number of elements (default 10000000)\n";
    std::cout << "  num_threads: threads to use (default = hardware concurrency)\n";
}

int main(int argc, char** argv) {
    size_t N = 10'000'000;
    unsigned num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;
    bool csv = false;
    unsigned repeat = 1;

    // simple arg parsing: [N] [num_threads] [--csv] [--repeat R]
    for (int i = 1; i < argc; ++i) {
        std::string a(argv[i]);
        if (a == "-h" || a == "--help") { usage(argv[0]); return 0; }
        if (a == "--csv" || a == "-c") { csv = true; continue; }
        if (a == "--repeat" && i + 1 < argc) { repeat = static_cast<unsigned>(std::atoi(argv[++i])); continue; }
        // otherwise positional
        if (N == 10'000'000) {
            N = static_cast<size_t>(std::strtoull(a.c_str(), nullptr, 10));
            continue;
        }
        if (num_threads == std::thread::hardware_concurrency() || num_threads == 0) {
            num_threads = static_cast<unsigned>(std::atoi(a.c_str()));
            if (num_threads == 0) num_threads = 1;
            continue;
        }
    }

    std::cout << "generating " << N << " ints, threads=" << num_threads << (csv ? " (csv)" : "") << "\n";
    auto v = generate_data(N);

    // accumulate timings over repeats
    long long s_sum = 0, a_sum = 0, p_sum = 0, m_sum = 0;
    long long s_val = 0, a_val = 0, p_val = 0, m_val = 0;
    bool ok = true;

    if (csv) {
        // CSV header
        std::cout << "mode,sum,time_ms\n";
    }

    for (unsigned r = 0; r < repeat; ++r) {
        auto s = single_thread_sum(v);
        auto a = atomic_parallel_sum(v, num_threads);
        auto p = partial_reduce_parallel_sum(v, num_threads);
        auto m = mutex_parallel_sum(v, num_threads);

        s_sum += s.second; a_sum += a.second; p_sum += p.second; m_sum += m.second;
        s_val = s.first; a_val = a.first; p_val = p.first; m_val = m.first;
        ok = ok && (s.first == a.first) && (s.first == p.first) && (s.first == m.first);

        if (csv) {
            std::cout << "single," << s.first << ',' << s.second << "\n";
            std::cout << "atomic," << a.first << ',' << a.second << "\n";
            std::cout << "partials," << p.first << ',' << p.second << "\n";
            std::cout << "mutex," << m.first << ',' << m.second << "\n";
        } else {
            print_result("単一スレッド", s);
            print_result("マルチスレッド(atomic)", a);
            print_result("マルチスレッド(partials->reduce)", p);
            print_result("マルチスレッド(mutex)", m);
            std::cout << "--- repeat " << (r+1) << " ---\n";
        }
    }

    if (!csv && repeat > 1) {
        std::cout << "平均 (" << repeat << "回):\n";
        std::cout << std::setw(30) << std::left << "単一スレッド" << ": sum=" << s_val << " avg_time_ms=" << (s_sum / repeat) << "\n";
        std::cout << std::setw(30) << std::left << "マルチスレッド(atomic)" << ": sum=" << a_val << " avg_time_ms=" << (a_sum / repeat) << "\n";
        std::cout << std::setw(30) << std::left << "マルチスレッド(partials->reduce)" << ": sum=" << p_val << " avg_time_ms=" << (p_sum / repeat) << "\n";
        std::cout << std::setw(30) << std::left << "マルチスレッド(mutex)" << ": sum=" << m_val << " avg_time_ms=" << (m_sum / repeat) << "\n";
    }

    std::cout << (ok ? "結果: OK" : "結果: NG (不一致)") << "\n";
    return ok ? 0 : 2;
}
