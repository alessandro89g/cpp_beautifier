#include "../include/FileReader.h"

using namespace std;

FileReader::FileReader(ifstream& file) {
    read_file(file);
}
FileReader::FileReader(const string& file_string) {
    read_file(file_string);
}

string FileReader::get_file_content() {
    return file_content;
}

void FileReader::read_file(ifstream& file) {
    string line;
    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }
    while (getline(file, line)) {
        file_content += line;
    }
    read_file(file_content);
}


void FileReader::read_file(const string& file_string) {
    stringstream text(file_string);
    string line;
    while (getline(text, line)) {
        if (line.find('(') != string::npos) {
            uint parenthesis = 0;
            for (const char& c : line) {
                if (c == '(') {
                    parenthesis++;
                } else if (c == ')') {
                    parenthesis--;
                }
            }
            if (parenthesis)
            while (parenthesis) {
                string new_line;
                getline(text, new_line);
                while (new_line[0] == ' ' || new_line[0] == '\t') {
                    new_line.erase(new_line.begin());
                }
                line += " " + new_line;
                if (new_line.find('(') != string::npos) {
                    parenthesis++;
                } else if (new_line.find(')') != string::npos) {
                    parenthesis--;
                }
            }
        }      
        text << line;
        text << '\n';
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
