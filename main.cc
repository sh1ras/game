#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <unistd.h>
#include <termios.h>
#include <csignal>
#include <cstdint>
#include "include/time.hh"

// =====================
// グローバル
// =====================
std::atomic<bool> running{true};
std::atomic<int64_t> enter_ms{-1};

std::mutex draw_mtx;
std::mutex log_mtx;
std::vector<std::string> push_times;

// =====================
// 判定
// =====================
enum class Judge { None, Good, Miss };

Judge judge_note(int64_t now, int64_t hit, int64_t range) {
    int64_t d = std::abs(now - hit);
    if (d <= range) return Judge::Good;
    if (now >= hit + range) return Judge::Miss;
    return Judge::None;
}

// =====================
// ノーツ
// =====================
struct Note {
    int64_t hit_ms;
    bool judged = false;
};

// =====================
// 端末制御（ENTERリアルタイム）
// =====================
struct TermGuard {
    termios oldt{};
    TermGuard() {
        tcgetattr(STDIN_FILENO, &oldt);
        termios t = oldt;
        t.c_lflag &= ~(ICANON | ECHO);
        t.c_cc[VMIN]  = 0;
        t.c_cc[VTIME] = 1;
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
    }
    ~TermGuard() {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
};

void signal_handler(int) {
    running.store(false);
}

// =====================
// 入力スレッド
// =====================
void input_thread() {
    char c;
    while (running.load()) {
        if (read(STDIN_FILENO, &c, 1) == 1 && c == '\n') {
            enter_ms.store(srs::mono_clock.now_ms());
        }
    }
}

// =====================
// 描画
// =====================
void draw(const std::vector<int>& notes, int line, int width) {
    std::lock_guard<std::mutex> lock(draw_mtx);
    std::system("clear");

    for (int i = 0; i < width; ++i) {
        if (i == line) std::cout << "|";
        else if (std::find(notes.begin(), notes.end(), i) != notes.end())
            std::cout << "■";
        else
            std::cout << " ";
    }
    std::cout << "\n";

    std::lock_guard<std::mutex> lk(log_mtx);
    for (auto& s : push_times) std::cout << s;
    std::cout << std::flush;
}

// =====================
// main
// =====================
int main() {
    std::signal(SIGINT, signal_handler);
    TermGuard tg;

    // ---- 設定 ----
    constexpr int WIDTH = 30;
    constexpr int LINE = 0;
    constexpr int START = WIDTH - 1;

    constexpr int HIT_MS = 2000;
    constexpr int GOOD_RANGE = 50;
    constexpr int64_t FRAME_MS = 16;      // 約60FPS
    constexpr int64_t END_AFTER_MS = 3500; // ★ n msで終了（ここ）

    int64_t start = srs::mono_clock.now_ms();

    // ---- ノーツ3つ ----
    std::vector<Note> notes = {
        { start + HIT_MS },
        { start + HIT_MS + 500 },
        { start + HIT_MS + 1000 }
    };

    std::thread th(input_thread);

    while (running.load()) {
        int64_t frame_start = srs::mono_clock.now_ms();

        // ---- 時間終了 ----
        if (frame_start - start >= END_AFTER_MS) {
            break;
        }

        std::vector<int> positions;

        // ---- ノーツ更新 ----
        for (auto& n : notes) {
            if (n.judged) continue;

            int64_t local = frame_start - (n.hit_ms - HIT_MS);
            int pos = START - (int)(local * START / HIT_MS);
            if (pos < LINE) pos = LINE;

            positions.push_back(pos);

            // 自然MISS（判定 +100ms）
            if (judge_note(frame_start, n.hit_ms, GOOD_RANGE) == Judge::Miss) {
                std::lock_guard<std::mutex> lk(log_mtx);
                push_times.emplace_back("MISS\n");
                n.judged = true;
            }
        }

        // ---- ENTER判定 ----
        int64_t e = enter_ms.exchange(-1);
        if (e != -1) {
            auto it = std::min_element(
                notes.begin(), notes.end(),
                [&](auto& a, auto& b) {
                    if (a.judged) return false;
                    if (b.judged) return true;
                    return std::abs(a.hit_ms - e) < std::abs(b.hit_ms - e);
                }
            );

            if (it != notes.end() && !it->judged) {
                Judge j = judge_note(e, it->hit_ms, GOOD_RANGE);
                std::lock_guard<std::mutex> lk(log_mtx);
                if (j == Judge::Good)
                    push_times.emplace_back(
                        "GOOD " + std::to_string(std::abs(e - it->hit_ms)) + "ms\n"
                    );
                else
                    push_times.emplace_back("MISS\n");

                it->judged = true;
            }
        }

        // ---- 描画 ----
        draw(positions, LINE, WIDTH);

        // ---- FPS補正 ----
        int64_t dt = srs::mono_clock.now_ms() - frame_start;
        if (dt < FRAME_MS)
            srs::clock::sleep(srs::mono_clock.ms(FRAME_MS - dt));
    }

    running.store(false);
    th.join();
    return 0;
}
