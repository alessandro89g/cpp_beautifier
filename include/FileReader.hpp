#ifndef FILE_READER_H
#define FILE_READER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <sstream>

#include "../include/RegexDefinitions.hpp"


class FileReader {
    #ifndef DEBUG
    #define DEBUG(...) std::cout << __VA_ARGS__ << std::endl;
    #endif // DEBUG
public:
    FileReader() = delete;
    FileReader(const std::string& file_string);
    FileReader(const FileReader& file_reader) = delete;
    FileReader(FileReader&& file_reader) = delete;

    std::string get_file_content() const;
    std::vector<std::string> get_blocks() const;

private:

    void break_into_blocks();

    void open_and_read_file(const std::string& file_path);

    void read_file(const std::string& file_string);

    void export_file(const std::string& file_path);

private:
    std::string file_content;
    std::vector<std::string> blocks;
};

#endif // FILE_READER_H