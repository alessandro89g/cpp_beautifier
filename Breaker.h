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

    enum Modifiers {
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
        vector<Modifiers> modifiers;
        bool definition_in_header;
    };
    
    struct Method : Definition {
        string owner_class;
        vector<string> args;
        string return_type;
    };

public:

    Method read_method(const string& body, uint line_start, uint line_end, Access access) {
        Method method;
        method.line_start = line_start;
        method.line_end = line_end;
        method.access = access;
        method.type = Type::METHOD;
        method.body = body;

        if (body.find_last_of(';') == body[body.length() - 1]) {
            method.definition_in_header = false;
        }
        if (body.find('{') != string::npos) {
            method.definition_in_header = true;
        }

        string head = body.substr(0, body.find("(") );
        while (head.find_last_of(" ") == head.length() - 1) {
            head = head.substr(0, head.length() - 1);
        }
        DEBUG("===============================")
        DEBUG("HEAD0: " << head)
        method.name = head.substr(head.find_last_of(" ") + 1);
        head = head.substr(0, head.find_last_of(" "));
        DEBUG("HEAD1: " << head)
        method.return_type = head.substr(head.find_last_of(" ") + 1);
        if (head.find_last_of(" ") != string::npos)
            head = head.substr(0, head.find_last_of(" "));
        else
            head = "";
        DEBUG("HEAD2: " << head)
        string modifiers = head;
        string args = body.substr(body.find("("), body.find(")") - body.find("(") + 2);
        DEBUG("HEAD: " << head)
        DEBUG("MODIFIERS: " << modifiers)
        DEBUG("RETURN TYPE: " << method.return_type)
        DEBUG("NAME: " << method.name)
        DEBUG("ARGS: " << args)
        cin.setf(ios::boolalpha);
        DEBUG("DEFINITION?: " << method.definition_in_header)



        return method;
    }


private:
    Modifiers read_modifiers(const string& body) {
        Modifiers modifier;
        return modifier;
    }

};

#endif // BREAKER_H