#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <sstream>

namespace srs {
    enum f_mode : u_int8_t {
        nomal = 0,
        binary = 1,
        empty = 100
    };

    class file {
    private:
        std::ofstream __ofs;
        const char* __f_name;
        srs::f_mode __f_mode;
        enum mode {
            nomal = 0,
            binary = 1,
            empty = 100
        };
    public:
        file();
        file(const char* name, srs::f_mode mode = srs::f_mode::nomal);
        file(const srs::file& me);
        ~file();

        srs::file& operator()(const char* name, srs::f_mode mode = srs::f_mode::empty);
        srs::file& operator()(const srs::file& file);

        template <typename T>
        srs::file& operator<(const T data);

        template <typename T>
        srs::file& operator<<(const T data);
        
        std::string load(const std::string& file) const;
        static std::string load(const std::string& file, const srs::f_mode& f_mode);
        srs::file& close();
    };
}
