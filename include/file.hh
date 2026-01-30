#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <filesystem>

namespace srs {
    class o_file {
        private:
            std::ofstream ofs;
            std::string path;

        public:
            o_file(const std::string& p, std::ios::openmode mode = std::ios::out | std::ios::app);
            ~o_file();

            inline void write(const std::string& data);
            inline void write_binary(const std::vector<char>& data);
            inline bool is_open() const;
            inline void flush();
            inline std::string get_path() const;
    };

    class i_file {
        private:
            std::ifstream ifs;
            std::string path;

        public:
            i_file(const std::string& p, std::ios::openmode mode = std::ios::in);
            ~i_file();

            inline std::string load();
            inline std::vector<char> load_binary();
            inline bool is_open() const;
            inline std::string get_path() const;
            inline void seek(std::streampos pos);
    };

    struct file {
        file() = default;
        ~file() = default;

        static inline bool write(const std::string& path, const std::string& data, std::ios::openmode mode = std::ios::app);
        static inline std::string load(const std::string& path, std::ios::openmode mode = std::ios::in);
        static inline bool write_binary(const std::string& path, const std::vector<char>& data, std::ios::openmode mode = std::ios::out);
        static inline std::vector<char> load_binary(const std::string& path, std::ios::openmode mode = std::ios::in);
        static inline std::uintmax_t size(const std::string& path);
        static inline bool exists(const std::string& path);
        static inline std::string filename(const std::string& path);
        static inline std::string directory(const std::string& path);

        static inline void open_ofs(std::ofstream& ofs, const std::string& path, std::ios::openmode mode);
        static inline void open_ifs(std::ifstream& ifs, const std::string& path, std::ios::openmode mode);
    };
}

