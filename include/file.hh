#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <filesystem>

namespace srs {

    std::vector<char> int_to_char_safe(const std::vector<int>& data) {
        std::vector<char> chardata;
        chardata.reserve(data.size());
        for (int v : data) {
            if (v < 0 || v > 255) {
                throw std::out_of_range("value > 1 byte");
            }
            chardata.push_back(static_cast<char>(v));
        }
        return chardata;
    }

    class o_file {
        private:
            std::ofstream ofs;
            std::string path;

        public:
            o_file(const std::string& p, std::ios::openmode mode = std::ios::out | std::ios::app);
            ~o_file();

            void write(const std::string& data);
            void write_binary(const std::vector<char>& data);
            bool is_open() const;
            void flush();
            std::string get_path() const;
    };

    class i_file {
        private:
            std::ifstream ifs;
            std::string path;

        public:
            i_file(const std::string& p, std::ios::openmode mode = std::ios::in);
            ~i_file();

            std::string load();
            std::vector<char> load_binary();
            bool is_open() const;
            std::string get_path() const;
            void seek(std::streampos pos);
    };

    struct file {
        file() = default;
        ~file() = default;

        static bool write(const std::string& path, const std::string& data, std::ios::openmode mode = std::ios::app);
        static std::string load(const std::string& path, std::ios::openmode mode = std::ios::in);
        static bool write_binary(const std::string& path, const std::vector<char>& data, std::ios::openmode mode = std::ios::out);
        static std::vector<char> load_binary(const std::string& path, std::ios::openmode mode = std::ios::in);
        static std::uintmax_t size(const std::string& path);
        static bool exists(const std::string& path);
        static std::string filename(const std::string& path);
        static std::string directory(const std::string& path);

        static void open_ofs(std::ofstream& ofs, const std::string& path, std::ios::openmode mode);
        static void open_ifs(std::ifstream& ifs, const std::string& path, std::ios::openmode mode);
    };
}

