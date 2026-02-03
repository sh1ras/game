#pragma once
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cstdint>
#include <optional>

    namespace srs {
        namespace audio_format {
            inline constexpr SDL_AudioFormat b08  = AUDIO_U8;
            inline constexpr SDL_AudioFormat b16  = AUDIO_S16SYS;
            inline constexpr SDL_AudioFormat b32  = AUDIO_S32SYS;
        }
        namespace channel {
            inline constexpr uint8_t mono   = 1;
            inline constexpr uint8_t stereo = 2;
        }
        

        class audio {
            private:
                SDL_AudioDeviceID __device_id = 0;
                uint8_t __volume = 0;

            public:
                static constexpr int sample_rate = 22050;
                static constexpr uint8_t max_volume = 128;
                inline static constexpr double hz(int midi) {
                    return 440.0 * pow(2.0, (midi - 69) / 12.0);
                }

                audio();
                ~audio();

                void volume(const uint8_t& v);
                void init(SDL_AudioFormat format, uint8_t channels, const std::optional<SDL_AudioDeviceID>& dv = std::nullopt);
                void h_init(SDL_AudioSpec& want, const std::optional<SDL_AudioDeviceID>& dv = std::nullopt);
                std::vector<std::string> devices(const bool& input);
                void pause(const bool p);
                template<typename... Freqs>
                void playback(int n, Freqs... freqs) {
                    std::vector<uint8_t> buf(n, 128);
                    double t;
                    for(int i=0; i<n; ++i) {
                        t = i / (double)srs::audio::sample_rate;
                        int sample = 0;
                        ((sample += 127 * sin(2.0 * M_PI * freqs * t)), ...);
                        // clamp
                        if(sample < -127) sample = -127;
                        if(sample > 127)  sample = 127;
                        buf[i] = 128 + sample;
                    }

                    SDL_ClearQueuedAudio(__device_id);
                    SDL_QueueAudio(__device_id, buf.data(), n);
                    SDL_PauseAudioDevice(__device_id, 0);
                }


                inline bool __is_setting(const SDL_AudioSpec& want, const SDL_AudioSpec& have) {
                    return want.freq == have.freq && want.format == have.format && want.channels == have.channels && want.samples  == have.samples;
                }
            };

    }
