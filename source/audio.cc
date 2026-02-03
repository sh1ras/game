#include "../include/audio.hh"
#include "../include/srs_sys.hh"
#include <cmath>
#include <iostream>

srs::audio::audio() : __volume(0), __device_id(0) {
    SDL_InitSubSystem(SDL_INIT_AUDIO);
}

srs::audio::~audio() {
    SDL_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void srs::audio::volume(const uint8_t& v) {
    __volume = v; 
    if (__volume > max_volume) {
        __volume = max_volume;
    }
}

void srs::audio::init(SDL_AudioFormat format, uint8_t channels, const std::optional<SDL_AudioDeviceID>& dv) {
    SDL_AudioSpec want{};
    want.freq     = 22050;
    want.format   = format;
    want.channels = channels;
    want.samples  = 512;
    want.userdata = nullptr;  

    h_init(want, dv);
}


void srs::audio::h_init(SDL_AudioSpec& want, const std::optional<SDL_AudioDeviceID>& dv) {
    SDL_AudioSpec have{};
    if(dv) {
        __device_id = *dv;
    } else {
        __device_id = SDL_OpenAudioDevice(nullptr, 0, &want, &have, 0);
        if(__device_id == 0) {
            std::cerr << "Failed to open audio device: " << SDL_GetError() << "\n";
            return;
        }
    }
    if(!__is_setting(want, have)) {
        std::cerr << "Audio settings do not match\n";
    }
}

std::vector<std::string> srs::audio::devices(const bool& input) {
    std::vector<std::string> list;
    int n = SDL_GetNumAudioDevices(input);
    for(int i=0; i<n; ++i) {
        const char* name = SDL_GetAudioDeviceName(i, input);
        if(name) list.push_back(name);
    }
    return list;
}

void srs::audio::pause(const bool p) {
    if(__device_id) SDL_PauseAudioDevice(__device_id, p ? 1 : 0);
}