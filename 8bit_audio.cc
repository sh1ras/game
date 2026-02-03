#pragma once
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cstdint>
#include <optional>
#include "include/srs_sys.hh"

namespace srs {

    class audio {
        private:
            SDL_AudioDeviceID __device_id = 0;
            uint8_t __volume = 0;
        public:
            static constexpr uint8_t max_volume = 128;

            audio() : __volume(0), __device_id(0) {
                SDL_InitSubSystem(SDL_INIT_AUDIO);
            }
            ~audio() {
                SDL_CloseAudio();
                SDL_QuitSubSystem(SDL_INIT_AUDIO);
            }

            void volume(uint8_t v) {
                __volume = v;
                if (__volume > max_volume) {
                    __volume = max_volume;
                }
            }
            inline bool __is_setting(const SDL_AudioSpec& want, const SDL_AudioSpec& have) {
                return want.freq == have.freq && want.format == have.format && want.channels == have.channels && want.samples  == have.samples;
            }
            void init_8bit() {
                SDL_AudioSpec want;
                want.freq = 22050;
                want.format = AUDIO_U8;
                want.channels = 1;
                want.samples = 512;
                want.userdata = nullptr;

                init(want);
                return;
            }

            void init(SDL_AudioSpec& want, std::optional<SDL_AudioDeviceID> dv = std::nullopt) {
                SDL_AudioSpec have;
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

            std::vector<std::string> devices(const bool& input) {
                std::vector<std::string> devices;
                int n = SDL_GetNumAudioDevices(input);
                for(int i = 0; i < n; ++i) {
                    const char* name = SDL_GetAudioDeviceName(i, input);
                    if(name) devices.push_back(name);
                }
                return devices;
            }

            void pause(const bool p) {
                SDL_PauseAudio(p);
                return;
            }

            void playback(int n, double freq) {
                std::vector<uint8_t> buf(n);
                SDL_ClearQueuedAudio(__device_id);
                SDL_QueueAudio(__device_id, buf.data(), n);
                SDL_PauseAudioDevice(__device_id, 0);
            }
    };

}
