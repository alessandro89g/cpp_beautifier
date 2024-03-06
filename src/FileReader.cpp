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
    DEBUG("File content: " << file_content)
    read_file(file_content);
    DEBUG("File content: " << file_content)
    abort();
}


void FileReader::read_file(const string& file_string) {
    // It has problemss with reading files with regex expressions because it cannot 
    // balance the parentheses as they do not need to be balanced in the file
    // Solution: ignore the parentheses in the strings
    stringstream text(file_string);
    string line, new_line;
    while (getline(text, line)) {
        line = remove_trailing_spaces(line);
        uint parentheses = parentheses_balance(line);
        if (parentheses)
            while (getline(text, new_line)) {
                new_line = remove_leading_trailing_spaces(new_line);
                line += " " + new_line;
                parentheses += parentheses_balance(new_line);
                if (parentheses == 0) {
                    break;
                }
                
            }  
        if (parentheses != 0) {
            throw std::runtime_error("Parentheses not balanced in file");
        }
        text << line;
        text << endl;
        line.clear();
    }
    file_content = text.str();
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