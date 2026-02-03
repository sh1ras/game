#include "include/audio.hh"
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

int main() {
    srs::audio a;

    // 音量設定
    a.volume(64);

    // 8bitモノラルで初期化（簡易INIT）
    a.init(srs::audio_format::b08, srs::channel::mono); // デフォルトは b08, mono

    // サンプルとして 1 秒間 440Hz（ラ）の音を再生
    int n = 22050; // 1秒分のサンプル
    double freq = 440.0;
    double freq2 = 220.0;

    a.playback(n, freq, freq2);

    // 音が終わるまで待機
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 停止


    std::cout << "Playback finished." << std::endl;

    return 0;
}
