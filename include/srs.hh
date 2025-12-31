#pragma once 

#include <iostream>
#include <random>
#include <string>

namespace srs{
    int64_t random(int min, int max) {
        if(min >= max) {
            throw std::invalid_argument("min must be less than max");
        } else {
            std::random_device rd;
            static std::mt19937 engine(rd());
            std::uniform_int_distribution<int> dist(min, max);
            return dist(engine);
        } return 0;
    }

    std::string command(std::string command_str) {
        char buffer[128];
        std::string result = "";
        FILE* pipe = popen((command_str + " 2>>./debug").c_str(), "r");
    
        if (!pipe) {
            throw std::runtime_error(command_str + "is failed!");
        }
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
           result += buffer;
        }
        pclose(pipe);
        return result;
    }

    [[maybe_unused]]
    inline size_t system_bits() {
        return sizeof(void*) * 8;
    }
}
