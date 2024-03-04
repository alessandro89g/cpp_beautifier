#ifndef FILE_READER_H
#define FILE_READER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <sstream>

using namespace std;

#define DEBUG(...)  cout << __VA_ARGS__ << endl;

#define _S "\\s+"                                           //  One or more of any whitespace character: ' ', '\t', '\v', '\f', '\r'
#define _s "\\s*"                                           // Zero or more of any whitespace character: ' ', '\t', '\v', '\f', '\r'
#define NAME_BASE "[a-zA-Z_][a-zA-Z0-9_]*"                       // Examples: "std" "filesystem" "Beautifier" "AccessSpecifier"
#define NAMESPACE "(?:" NAME_BASE _s "::" _s ")*"                // Examples: "std::" "std::filesystem::"
#define TYPE_BASE "(?:" NAMESPACE NAME_BASE _s "(<.+?>)?)"    
#define CONST "(?:const" _S ")?"
#define TYPE "(" CONST TYPE_BASE "(?:\\*|&|&&)?)" _s  // Examples: "const std::string" "const std::filesystem::path" "const Beautifier*" "const vector<Beautifier>&"
#define ARG "(((?:" TYPE _S NAME_BASE ")))"
#define ARGS "\\((" _s ARG ",?)*\\)"         //"\\((" _s ARG ",?)*\\)"                  // Examples: "(const std::string name, const std::filesystem::path header, const vector<Beautifier>& classes)"
#define PRE_MODIFIERS "(static|virtual|abstract|inline|mutable|explicit|extern|noexcept|constexpr)"
#define POST_MODIFIERS  "(?:\\s+(?:const|throws|volatile|noexcept|throws))*"
#define METHOD_BASE  "\\b(?!" PRE_MODIFIERS "\\b)" TYPE "\\b(" NAME_BASE ")" _s ARGS             // Examples: "const std::string getHeader()" "const std::filesystem::path getSource()" "const vector<Beautifier>& getClasses()"

#define METHOD "(" PRE_MODIFIERS _s ")?" METHOD_BASE  POST_MODIFIERS "[;|{]" // Examples: "const std::string getHeader() const" "const std::filesystem::path getSource() const" "const vector<Beautifier>& getClasses() const"

#define ACCESS_SPECIFIER "(public|protected|private)"

#define CLASS_NAME "\\h*class\\s+(" NAME_BASE ")(\\s*:\\s+(" ACCESS_SPECIFIER "?\\s*" TYPE_BASE "\\,?\\s?)*\\s*)?\\s*(;|\\{)"

#define MEMBER TYPE _S "(" NAME_BASE "))\\h*[,|\\)|;])"


class FileReader {
public:
    FileReader() = delete;
    FileReader(const string& file_path) {
        read_file(file_path);
    }

    string get_file_content() {
        return file_content;
    }

    vector<string> get_methods() {
        return methods;
    }

    vector<string> get_classes() {
        return classes;
    }

private:

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
        find_methods();
        find_classes();
    }

    void export_file(const string& file_path) {
        ofstream file(file_path);
        if (!file.is_open()) {
            throw std::runtime_error("File not found: " + file_path);
        }
        file << file_content;
    }

    void find_methods() {
        const string pattern_string = METHOD;
        smatch match;
        regex pattern(pattern_string);
        string text = file_content;

        while(regex_search(text,match,pattern)) {
            methods.push_back(match.str());
            text = match.suffix();
        }
    }

    void find_classes() {
        const string pattern_string = CLASS_NAME;
        smatch match;
        regex pattern(pattern_string);
        string text = file_content;

        while(regex_search(text,match,pattern)) {
            classes.push_back(match.str());
            text = match.suffix();
        }
    }
    


    
    string file_content;
    vector<string> classes;
    vector<string> methods;
    vector<string> members;
    vector<string> namespaces;

};

#endif // FILE_READER_H