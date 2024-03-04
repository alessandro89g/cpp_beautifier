#ifndef BREAKER_H
#define BREAKER_H

#include "RegexDefinitions.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <regex>
#include <memory>

using namespace std;

class Breaker {
    #define un_ptr(x) unique_ptr<x>
    #define DEBUG(...) cout << __VA_ARGS__ << endl;
public:
    enum Type {
        INCLUDE,
        CLASS,
        CONSTRUCTOR,
        DESTRUCTOR,
        METHOD,
        MEMBER
    };

    enum Modifier {
        STATIC,
        VIRTUAL,
        ABSTRACT,
        INLINE,
        MUTABLE,
        EXPLICIT,
        EXTERN,
        NOEXCEPT,
        CONSTEXPR
    };

    enum Access {
        PUBLIC,
        PROTECTED,
        PRIVATE
    };

    struct Definition {
        string name;
        string body;
        uint line_start;
        uint line_end;
        Type type;
        Access access;
        vector<Modifier> pre_modifiers;
        vector<Modifier> post_modifiers;
        bool definition_in_header;
    };
    
    struct Method : Definition {
        string owner_class;
        vector<string> args;
        string return_type;
    };

public:

    Method read_method(const string& string_method, uint line_start, Access access) {
        DEBUG("===============================")
        DEBUG(string_method)
        Method method;
        method.line_start = line_start;
        method.line_end = line_start + lines_in_block(string_method);
        method.access = access;
        method.type = Type::METHOD;
        method.body = read_body(string_method);

        if (string_method.find_last_of(';') == string_method[string_method.length() - 1]) {
            method.definition_in_header = false;
        }
        if (string_method.find('{') != string::npos) {
            method.definition_in_header = true;
        }

        string head = string_method.substr(0, string_method.find("(") );
        while (head.find_last_of(" ") == head.length() - 1) {
            head = head.substr(0, head.length() - 1);
        }
        // DEBUG("HEAD0: " << head)
        method.name = head.substr(head.find_last_of(" ") + 1);
        head = head.substr(0, head.find_last_of(" "));
        // DEBUG("HEAD1: " << head)
        method.return_type = head.substr(head.find_last_of(" ") + 1);
        if (head.find_last_of(" ") != string::npos)
            head = head.substr(0, head.find_last_of(" "));
        else
            head = "";
        // DEBUG("HEAD2: " << head)
        method.pre_modifiers = read_modifiers(head);
        method.args = read_args(string_method.substr(string_method.find("("), string_method.find(")") - string_method.find("(") + 2));
        // DEBUG("HEAD: " << head)
        // cout << "MODIFIERS: ";
        // for (Modifier& modifier : method.pre_modifiers) {
            // cout << modifier << " ";
        // }
        // cout << endl;
        // DEBUG("RETURN TYPE: " << method.return_type)
        // DEBUG("NAME: " << method.name)
        // cout << "ARGS: ";
        // print_vector(method.args);
        // DEBUG("DEFINITION?: " << method.definition_in_header)
        // DEBUG("LINE START: " << method.line_start)
        // DEBUG("LINE END: " << method.line_end)

        DEBUG(method_to_string(method, true))



        return method;
    }


private:

    vector<Modifier> read_modifiers(const string& string_modifiers) {
        vector<Modifier> modifiers;
        vector<string> tokens = split(string_modifiers, " ");
        for (string& token : tokens) {
            clear_string(token);
            if (token == "static") {
                modifiers.push_back(Modifier::STATIC);
            } else if (token == "virtual") {
                modifiers.push_back(Modifier::VIRTUAL);
            } else if (token == "abstract") {
                modifiers.push_back(Modifier::ABSTRACT);
            } else if (token == "inline") {
                modifiers.push_back(Modifier::INLINE);
            } else if (token == "mutable") {
                modifiers.push_back(Modifier::MUTABLE);
            } else if (token == "explicit") {
                modifiers.push_back(Modifier::EXPLICIT);
            } else if (token == "extern") {
                modifiers.push_back(Modifier::EXTERN);
            } else if (token == "noexcept") {
                modifiers.push_back(Modifier::NOEXCEPT);
            } else if (token == "constexpr") {
                modifiers.push_back(Modifier::CONSTEXPR);
            }
        }


        return modifiers;
    }

    void clear_string(string& str) {
        if (str == "" || str == " ") {
            return;
        }
        while (str.find_first_of(" ") == 0) {
            str = str.substr(1);
        }
        while (str.find_last_of(" ") == str.length() - 1) {
            str = str.substr(0, str.length() - 1);
        }
    }

    vector<string> split(const string& str, const string& delimiter) {
        vector<string> tokens;
        size_t pos = 0;
        size_t last_pos = 0;
        while ((pos = str.find(delimiter, last_pos)) != string::npos) {
            tokens.push_back(str.substr(last_pos, pos - last_pos));
            last_pos = pos + 1;
        }
        tokens.push_back(str.substr(last_pos));
        return tokens;
    }

    uint lines_in_block(const string& block) {
        uint lines = 0;
        for (const char& c : block) {
            if (c == '\n') {
                lines++;
            }
        }
        return lines;
    }


    vector<string> read_args(string string_args) {
        string_args = string_args.substr(1, string_args.find(")")-1);
        vector<string> args = split(string_args, ",");
        for (string& arg : args) {
            clear_string(arg);
        }
        args.erase(remove_if(args.begin(), args.end(), [](const string& str) { return str == ""; }), args.end());
        return args;
    }

    string read_body(const string& string_method) {
        if (string_method.find("{") == string::npos) {
            return "";
        }
        string body = string_method.substr(string_method.find("{") + 1, string_method.find_last_of("}") - string_method.find("{") - 1);
        return body;
    }



    template<typename T>
    void print_vector(const vector<T>& vec, ostream& os = cout) {
        os << "( ";
        for (size_t i = 0; i < vec.size(); i++) {
            os << vec[i];
            if (i != vec.size() - 1)
                os << ", ";
        }
        os << " )\n";
    }

    friend ostream& operator<<(ostream& os, const Access& access) {
        switch (access) {
            case Access::PUBLIC:
                os << "public";
                break;
            case Access::PROTECTED:
                os << "protected";
                break;
            case Access::PRIVATE:
                os << "private";
                break;
            default:
                throw std::runtime_error("Tring to print unknown access");
        }
        return os;
    }

    string method_to_string(const Method& method, bool more_info = false) {
        stringstream ss;
        for (const Modifier& modifier : method.pre_modifiers) {
            ss << modifier << " ";
        }
        ss << method.return_type << " " << method.name;
        if (method.args.size() == 0) {
            ss << "()";
        }
        else {
            ss << "( ";
            for (size_t i = 0; i < method.args.size(); i++) {
                ss << method.args[i];
                if (i != method.args.size() - 1)
                    ss << ", ";
            }
            ss << " )";
        }
        for (const Modifier& modifier : method.post_modifiers) {
            ss << " " << modifier;
        }
        if (more_info) {
            if (method.body != "") 
                ss << " {\n " << method.body << "\n}\n\n";
            else 
                ss << ";\n\n";
            ss << " Access: " << method.access << '\n';
            ss << " Line start: " << method.line_start << "\n";
            ss << " Line end: " << method.line_end << "\n";
        }

        return ss.str();
    }


    friend ostream& operator<<(ostream& os, const Modifier& modifier) {
        switch (modifier) {
            case Modifier::STATIC:
                os << "static";
                break;
            case Modifier::VIRTUAL:
                os << "virtual";
                break;
            case Modifier::ABSTRACT:
                os << "abstract";
                break;
            case Modifier::INLINE:
                os << "inline";
                break;
            case Modifier::MUTABLE:
                os << "mutable";
                break;
            case Modifier::EXPLICIT:
                os << "explicit";
                break;
            case Modifier::EXTERN:
                os << "extern";
                break;
            case Modifier::NOEXCEPT:
                os << "noexcept";
                break;
            case Modifier::CONSTEXPR:
                os << "constexpr";
                break;
            default:
                throw std::runtime_error("Tring to print unknown modifier");
        }
        return os;
    }
    

};

#endif // BREAKER_H