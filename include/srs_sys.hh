#pragma

#include <iostream>
#include <filesystem>

namespace srs {
    [[maybe_unused]]
    inline std::string static_working_directory() {
        static std::string dir = std::filesystem::current_path().string();
        return dir;
    }
    
    [[maybe_unused]]
    inline std::string working_directory() {
        std::string dir = std::filesystem::current_path().string();
        return dir;
    }
}