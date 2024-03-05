#ifndef BREAKER_H
#define BREAKER_H

#include "../include/RegexDefinitions.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <regex>
#include <memory>

class Breaker {
    #define un_ptr(x) std::unique_ptr<x>
    #ifndef DEBUG
    #define DEBUG(...) std::cout << __VA_ARGS__ << std::endl;
    #endif // DEBUG
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
        CONST,
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
        Type type;
        Access access;
        std::string name;
        std::string implementation_body;
        std::string body;
        uint line_start;
        uint line_end;
        std::vector<Modifier> pre_modifiers;
        std::vector<Modifier> post_modifiers;
        bool definition_in_header;
    };
    
    struct Include {
        std::string name;
        bool is_system;
    };

    struct Method : Definition {
        std::string owner_class;
        std::vector<std::string> args;
        std::string return_type;
    };

    struct Constructor : Definition {
        std::vector<std::string> args;
        std::vector<std::string> initializer_list;
        bool is_deleted;
    };

    struct Destructor : Definition {
        bool is_virtual;
        bool is_deleted;
    };

    struct Member : Definition {
        std::string type;
    };

public:

    Method read_method(const std::string& string_method, uint line_start, Access access);

    std::string read_body(const std::string& string_method);

private:

    std::vector<Modifier> read_modifiers(const std::string& string_modifiers);

    void clear_string(std::string& str);

    std::vector<std::string> split(const std::string& str, const std::string& delimiter);

    uint lines_in_block(const std::string& block);

    std::vector<std::string> read_args(std::string string_args);

    std::string method_to_string(const Method& method, bool more_info);


    template<typename T>
    void print_vector(const std::vector<T>& vec, std::ostream& os = std::cout) {
        os << "( ";
        for (size_t i = 0; i < vec.size(); i++) {
            os << vec[i];
            if (i != vec.size() - 1)
                os << ", ";
        }
        os << " )\n";
    }
    friend std::ostream& operator<<(std::ostream& os, const Access& access);
    friend std::ostream& operator<<(std::ostream& os, const Modifier& modifier);
    

};

#endif // BREAKER_H