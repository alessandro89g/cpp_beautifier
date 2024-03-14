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
    enum Type {
        HEADER,
        SOURCE
    };

    FileReader() = delete;
    FileReader(const std::string& file_string);
    FileReader(const FileReader& file_reader) = delete;
    FileReader(FileReader&& file_reader) = delete;

    std::string get_file_content(bool original = false) const;

    std::string get_file_name() const;

    void export_file(const std::string& file_path, bool original = false) const;

    Type get_type() const { return type; }

protected:

    void open_and_read_file(const std::string& file_path);

    void modify_file(const std::string& file_string);

private:
    std::string _file_path;
    std::string file_content_original;
    std::string file_content_modified;
    Type type;
};

#endif // FILE_READER_H