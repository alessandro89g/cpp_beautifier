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
    FileReader(std::ifstream& file);
    FileReader(const std::string& file_string);
    FileReader(const FileReader& file_reader) = delete;
    FileReader(FileReader&& file_reader) = delete;

    std::string get_file_content();

protected:

    void read_file(std::ifstream& file);

    void read_file(const std::string& file_string);

    void export_file(const std::string& file_path);

protected:
    std::string file_content;

};

#endif // FILE_READER_H