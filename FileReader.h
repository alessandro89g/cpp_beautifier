#ifndef FILE_READER_H
#define FILE_READER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <sstream>

#include "RegexDefinitions.hpp"

using namespace std;

#define DEBUG(...)  cout << __VA_ARGS__ << endl;


class FileReader {
public:
    FileReader() = delete;
    FileReader(ifstream& file) {
        read_file(file);
    }
    FileReader(const string& file_string) {
        read_file(file_string);
    }
    FileReader(const FileReader& file_reader) = delete;
    FileReader(FileReader&& file_reader) = delete;

    string get_file_content() {
        return file_content;
    }

    

protected:

    void read_file(ifstream& file) {
        string line;
        if (!file.is_open()) {
            throw std::runtime_error("File not found");
        }
        while (getline(file, line)) {
            file_content += line;
        }
        read_file(file_content);
    }


    void read_file(const string& file_string) {
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

    void export_file(const string& file_path) {
        ofstream file(file_path);
        if (!file.is_open()) {
            throw std::runtime_error("File not found: " + file_path);
        }
        file << file_content;
    }
    string file_content;

};

#endif // FILE_READER_H