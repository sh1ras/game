#include "../include/file.hh"

srs::file::file() : __f_name(""), __f_mode(srs::f_mode::nomal) {}
srs::file::file(const char* name, srs::f_mode mode) : __f_name(name), __f_mode(mode) {}
srs::file::file(const srs::file& me) : __f_name(me.__f_name), __f_mode(me.__f_mode) {}
srs::file::~file() {}

srs::file& srs::file::operator()(const char* name, srs::f_mode mode) {
    __f_name = name;
    if(mode != srs::f_mode::empty) {
        __f_mode = mode;
    } return *this;
}

srs::file& srs::file::operator()(const srs::file& file) {
    __f_name = file.__f_name;
    __f_mode = file.__f_mode;
    return *this;
}

template <typename T>
srs::file& srs::file::operator<(const T data) {
    if (__f_mode == srs::f_mode::nomal) {
        __ofs.open(__f_name, std::ios::out | std::ios::trunc);
        if (!__ofs.is_open()) {
            throw std::runtime_error("Failed to open file: " + std::string(__f_name));
        }
        __ofs << data;
        __ofs.close();
    } else if (__f_mode == srs::f_mode::binary) {
        __ofs.open(__f_name, std::ios::binary | std::ios::out | std::ios::trunc);
        if (!__ofs.is_open()) {
            throw std::runtime_error("Failed to open file: " + std::string(__f_name));
        }
        __ofs.write(reinterpret_cast<const char*>(&data), sizeof(T));
        __ofs.close();
    }
    return *this;
}

template <typename T>
srs::file& srs::file::operator<<(const T data) {
    if (__f_mode == srs::f_mode::nomal) {
        __ofs.open(__f_name, std::ios::app);
        if (!__ofs.is_open()) {
            throw std::runtime_error("Failed to open file: " + std::string(__f_name));
        }
 
        __ofs << data;
        __ofs.close();
    } else if (__f_mode == srs::f_mode::binary) {
        __ofs.open(__f_name, std::ios::binary | std::ios::app);
        if (!__ofs.is_open()) {
            throw std::runtime_error("Failed to open file: " + std::string(__f_name));
        }
        __ofs.write(reinterpret_cast<const char*>(&data), sizeof(T));
        __ofs.close();
    }
    return *this;
}

inline std::string srs::file::load(const std::string& file) const {
    return this->load(file, __f_mode);
}

std::string srs::file::load(const std::string& file, const srs::f_mode& f_mode) {
    std::ios_base::openmode mode = std::ios::in;

    if (f_mode == srs::f_mode::binary) {
        mode |= std::ios::binary;
    } else if (f_mode == srs::f_mode::empty) {
        throw std::runtime_error("srs::file::load f_mode is empty");
    } else if (f_mode != srs::f_mode::nomal) {
        throw std::runtime_error("srs::file::load unknown f_mode. f_mode = " + std::to_string(static_cast<u_int8_t>(f_mode)));
    }

    std::ifstream ifs(file, mode);
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open file: " + file);
    }

    return std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
}


srs::file& srs::file::close() {
    if(__ofs.is_open()) {
        __ofs.close();
    } return *this;
}

std::string get_line(int index, const std::string& data) {
    std::istringstream iss(data);
    std::string line;

    for (int i = 0; i <= index; ++i) {
        if (!std::getline(iss, line))
            throw std::runtime_error("line index out of range");
    }

    return line;
}