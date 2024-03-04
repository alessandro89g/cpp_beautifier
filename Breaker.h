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
        Modifiers modifiers;
        bool definition_in_header;
    };
    
    struct Method : Definition {
        vector<string> args;
        string return_type;
    };



    vector<un_ptr(Definition)> _definitions;

};

#endif // BREAKER_H