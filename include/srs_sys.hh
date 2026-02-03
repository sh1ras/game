#pragma once

#include <filesystem>
#include <string>

#ifndef SCOPE
    #define SCOPE __PRETTY_FUNCTION__
#endif


namespace srs {
    inline constexpr bool error = 1;
    inline constexpr bool pass = 0;
    
    namespace sys {
        /* wd = working directory */
        inline const std::string& startup_wd() {
            static const std::string dir = std::filesystem::current_path().string();
            return dir;
        }
        /* wd = working directory */
        inline std::string now_wd() {
            return std::filesystem::current_path().string();
        }
    }
}
