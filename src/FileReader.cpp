#include "../include/FileReader.hpp"
#include "../include/utilities.hpp"

using namespace std;

FileReader::FileReader(const string& file_string) {
    open_and_read_file(file_string);
}

string FileReader::get_file_content(bool original) const {
    return (original ? file_content_original : file_content_modified);
}

void FileReader::open_and_read_file(const string& file_path) {
    string line;
    ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }
    file_content_original.clear();
    while (getline(file, line)) {
        file_content_original += line + '\n';
    }
    file.close();
    file_content_modified = remove_trailing_new_lines(file_content_original);
    modify_file(file_content_modified);
}


void FileReader::modify_file(const string& file_string) {
    string line;
    vector<string> lines = string_split(file_string);
    for (size_t i = 0; i < lines.size(); i++) {
        line = remove_trailing_spaces(lines[i]);
        uint parentheses = parentheses_balance(line);
        while (parentheses != 0) {
            ++i;
            if (lines[i] == "") {
                continue;
            }
            string new_line = remove_leading_trailing_spaces(lines[i]);
            line += " " + new_line;
            parentheses += parentheses_balance(new_line);
            if (parentheses == 0) {
                break;
            }
        }
        file_content_modified += line + '\n';
    }
    file_content_modified.erase(file_content_modified.end() - 1);
}


void FileReader::export_file(const std::string& file_path, bool original) const {
    std::ofstream file(file_path, std::ios::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("Could not generate: " + file_path);
    }
    if (original) {
        file << file_content_original;
    } else {
        file << file_content_modified;
    }
    file.close();
}
    