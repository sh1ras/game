#pragma once

#include "file.hh"
#include "../source/file.cc"
#include "srs_str.hh"

namespace srs {
    struct ban {
        std::vector<std::string> ban_list;
        void reload(const std::string& ban_list_file) {
            std::string file_data = srs::file::load(ban_list_file, srs::f_mode::nomal);

            ban_list = srs::str::split_line(file_data);
            return;
        }

        bool ban_check(const std::string& str, const char* ban_list_file = nullptr) {
            if(ban_list.empty()) {
                if(ban_list_file == nullptr || std::string(ban_list_file).empty()) {
                    throw std::runtime_error("ban_list_file is nullptr or empty");
                } else {
                    reload(ban_list_file);
                }   
            } for (const std::string& ban_str : ban_list) {
                if (str.find(ban_str) != std::string::npos) {
                    return true;
                }
            }
            return false;
        }
    };
}
