#include "../include/file.hh"
#include <sstream>

srs::o_file::o_file(const std::string& p, std::ios::openmode mode) : path(p) {
    ofs.open(path, mode);
    if (!ofs.is_open()) {
        throw std::runtime_error("o_file can't open " + path);
    }
}

srs::o_file::~o_file() {
    if (ofs.is_open()) {
        ofs.close();
    }
}

void srs::o_file::write(const std::string& data) {
    ofs << data;
}

void srs::o_file::write_binary(const std::vector<char>& data) {
    ofs.write(data.data(), data.size());
}

bool srs::o_file::is_open() const {
    return ofs.is_open();
}

void srs::o_file::flush() {
    ofs.flush();
}

std::string srs::o_file::get_path() const {
    return path;
}

// ------------------------

srs::i_file::i_file(const std::string& p, std::ios::openmode mode)
    : path(p)
{
    ifs.open(path, mode);
    if (!ifs.is_open()) {
        throw std::runtime_error("i_file can't open " + path);
    }
}

srs::i_file::~i_file() {
    if (ifs.is_open()) {
        ifs.close();
    }
}

std::string srs::i_file::load() {
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    return buffer.str();
}

std::vector<char> srs::i_file::load_binary() {
    ifs.seekg(0, std::ios::end);
    std::streamsize size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    ifs.read(buffer.data(), size);
    return buffer;
}

bool srs::i_file::is_open() const {
    return ifs.is_open();
}

std::string srs::i_file::get_path() const {
    return path;
}

void srs::i_file::seek(std::streampos pos) {
    ifs.seekg(pos);
}

// ------------------------ srs::file 関数

bool srs::file::write(const std::string& path, const std::string& data, std::ios::openmode mode) {
    std::ofstream ofs;
    open_ofs(ofs, path, mode);
    ofs << data;
    return true;
}

std::string srs::file::load(const std::string& path, std::ios::openmode mode) {
    std::ifstream ifs;
    open_ifs(ifs, path, mode);
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    return buffer.str();
}

bool srs::file::write_binary(const std::string& path, const std::vector<char>& data, std::ios::openmode mode) {
    std::ofstream ofs;
    open_ofs(ofs, path, mode | std::ios::binary);
    ofs.write(data.data(), data.size());
    return true;
}

std::vector<char> srs::file::load_binary(const std::string& path, std::ios::openmode mode) {
    std::ifstream ifs;
    open_ifs(ifs, path, mode | std::ios::binary);
    ifs.seekg(0, std::ios::end);
    std::streamsize size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    ifs.read(buffer.data(), size);
    return buffer;
}

std::uintmax_t srs::file::size(const std::string& path) {
    if (!exists(path)) {
        return 0;
    }
    return std::filesystem::file_size(path);
}

bool srs::file::exists(const std::string& path) {
    return std::filesystem::exists(path);
}

std::string srs::file::filename(const std::string& path) {
    return std::filesystem::path(path).filename().string();
}

std::string srs::file::directory(const std::string& path) {
    return std::filesystem::path(path).parent_path().string();
}

void srs::file::open_ofs(std::ofstream& ofs, const std::string& path, std::ios::openmode mode) {
    ofs.open(path, mode);
    if (!ofs.is_open()) {
        throw std::runtime_error("srs::file::open_ofs " + path + " can't open");
    }
}

void srs::file::open_ifs(std::ifstream& ifs, const std::string& path, std::ios::openmode mode) {
    ifs.open(path, mode);
    if (!ifs.is_open()) {
        throw std::runtime_error("srs::file::open_ifs " + path + " can't open");
    }
}
