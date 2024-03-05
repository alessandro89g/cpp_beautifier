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
    read_file(file_content);
}


void FileReader::read_file(const string& file_string) {
    stringstream text(file_string);
    string line;
    while (getline(text, line)) {
        uint parenthesis = parentheses_balance(line);
        while (parenthesis) {
            string new_line;
            getline(text, new_line);
            while (new_line[0] == ' ' || new_line[0] == '\t') {
                new_line.erase(new_line.begin());
            }
            line += " " + new_line;
            parenthesis += parentheses_balance(new_line);
        }  
        text << line;
        text << '\n';
    }
    file_content = text.str();
    file_content.erase(file_content.end() - 1);
    DEBUG(file_content)
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