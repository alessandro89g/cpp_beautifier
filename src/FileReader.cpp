#include "../include/FileReader.hpp"
#include "../include/utilities.hpp"

using namespace std;

FileReader::FileReader(const string& file_string) {
    open_and_read_file(file_string);
}

string FileReader::get_file_content() const {
    return file_content;
}

vector<string> FileReader::get_blocks() const {
    return blocks;
}

void FileReader::open_and_read_file(const string& file_path) {
    string line;
    ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }
    while (getline(file, line)) {
        file_content += line + '\n';
    }
    file.close();
    file_content = remove_trailing_new_lines(file_content);
    read_file(file_content);
}


void FileReader::read_file(const string& file_string) {
    string line;
    vector<string> lines = string_split(file_string);
    file_content.clear();
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
        file_content += line + '\n';
    }
    file_content.erase(file_content.end() - 1);
}

void FileReader::export_file(const string& file_path) {
    ofstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("File not found: " + file_path);
    }
    file << file_content;
}

void FileReader::break_into_blocks() {
    string block;
    string line;
    stringstream text(file_content);
    size_t brakets = 0;
    uint n_lines = 0;
    while (getline(text, line, '\n')) {
        if (line.find('{') != string::npos) {
            brakets++;
        }
        if (line.find('}') != string::npos) {
            brakets--;
        }
        block += line + '\n';
        if (!brakets) {
            blocks.push_back(block);
            block.clear();
        }
    }
}