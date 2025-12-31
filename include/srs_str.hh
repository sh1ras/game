#pragma once

#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

namespace srs::str {

    std::string get_line(std::size_t index, const std::string& data) {
        std::istringstream iss(data);
        std::string line;

        for (std::size_t i = 0; i <= index; ++i) {
            if (!std::getline(iss, line))
                throw std::runtime_error("line index out of range");
        }

        return line;
    }

    std::vector<std::string> token(const std::string& data, char delimiter = ' ') {
        std::vector<std::string> result;
        std::string tok;
        std::istringstream iss(data);

        while (std::getline(iss, tok, delimiter))
            result.push_back(tok);

        return result;
    }

    inline std::vector<std::string> split_line(const std::string& data) {
        return token(data, '\n');
    }

}
