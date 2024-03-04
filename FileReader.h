#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <sstream>

using namespace std;





class FileReader {
public:
    FileReader() = delete;
    FileReader(const string& file_path) {
        read_file(file_path);
    }

    void read_file(const string& file_path) {
        ifstream file(file_path);
        if (!file.is_open()) {
            throw std::runtime_error("File not found: " + file_path);
        }
        string line;
        while (getline(file, line)) {
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
                    getline(file, new_line);
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
            file_content += line;
            file_content += '\n';
        }
        file_content.erase(file_content.end() - 1);
    }

    void export_file(const string& file_path) {
        ofstream file(file_path);
        if (!file.is_open()) {
            throw std::runtime_error("File not found: " + file_path);
        }
        file << file_content;
    }

    bool is_method(const string& line) {
        return line.find('(') != string::npos;
    }


    
    string file_content;
    vector<string> methods;
    vector<string> members;
    vector<string> namespaces;

};
