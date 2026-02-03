#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <memory>
#include <filesystem>
#include <optional>
#include <vector>

#include <nlohmann/json.hpp>
#include "debug.cc"

std::string get_terminal() {
    static std::string dir = std::filesystem::current_path().string();
    return dir;
}

inline bool is_reject_input(const std::string& output) {
    return output.find("!") == std::string::npos || output.empty();
}

std::vector<std::string> get_serihu(const std::string& key) {
    std::vector<std::string> ret;
    std::string file_path = get_terminal() + "/json/404.json"; 
    srs::log("File Path: " + file_path);
    std::ifstream file(file_path);
    file << nlohmann::json::parse(file);
    if (!file) { 
        throw std::runtime_error("404 : File Not Found"); 
    }
    return {""};
}

std::optional<std::string> get_user_output(u_int8_t& call_counter, const uint8_t limit = 2) {
    static std::vector<std::string> serihu = get_serihu("get_user_output");
    if(call_counter <= limit) {
        std::string output = "";
        std::cout << "Enter your output: ";
        std::getline(std::cin, output);
        if (is_reject_input(output)) {
            std::cout << "sry, try again.\n";
            call_counter += 1;
            return get_user_output(call_counter, limit);
        } return output;
    } return std::nullopt;
}

int main() {
/*     u_int8_t counter = 0;
    std::optional<std::string> user_output = get_user_output(counter);
    if(user_output == std::nullopt) {
        std::cout << "Too many attempts! \n";
        return 1;
    } else {
        std::cout << "User: " << (*user_output) << "\n";
    } */
    try {
        get_serihu("get_user_output");
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        srs::log(e.what());
        return 1;
    }
    return 0;
}
