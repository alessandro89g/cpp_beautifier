#ifndef BREAKER_H
#define BREAKER_H

#include "../include/RegexDefinitions.hpp"
#include "../include/utilities.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <regex>
#include <memory>
#include <queue>

class Breaker {
    #define un_ptr(x) std::shared_pointer<x>
    #define mk_ptr(x) std::make_shared<x>
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
        MEMBER,
        OTHER
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

    struct Block {
        Block() = default;
        Block(const std::string& str, uint line_start) : body(str), line_start(line_start) {
            line_end = line_start + Breaker::get_instance().lines_in_block(str);
        }
        std::string body;
        uint line_start;
        uint line_end;
        Block& operator += (const std::string& str) {
            body += str;
            line_end += Breaker::get_instance().lines_in_block(str);
            return *this;
        }
        Block& operator = (const std::string& str) {
            body = str;
            line_end = line_start + Breaker::get_instance().lines_in_block(str);
            return *this;
        }
        explicit operator const char* () const { return body.c_str(); }
    };

    struct Definition {
        Type type;
        Access access;
        std::string name;
        std::string implementation_body;
        Block body;
        std::vector<Modifier> pre_modifiers;
        std::vector<Modifier> post_modifiers;
        bool definition_in_header;
    };
    
    struct Include {
        std::string name;
        bool is_system;
    };

    struct Method : public Definition {
        std::string owner_class;
        std::vector<std::string> args;
        std::string return_type;
    };

    struct Constructor : public Definition {
        std::vector<std::string> args;
        std::vector<std::string> initializer_list;
        bool is_deleted;
    };

    struct Destructor : public Definition {
        bool is_virtual;
        bool is_deleted;
    };

    struct Member : public Definition {
        std::string type;
    };

public:

    Method read_method(const std::string& string_method, uint line_start, Access access) const;
    Method read_method(const Block& block, Access access) const;

    std::string read_body(const std::string& string_method) const;

    std::queue<Block> split_in_blocks(const std::string& str) const;

    static Breaker& get_instance();

protected:
    Breaker() = default;
    Breaker(const Breaker& breaker) = delete;
    Breaker(Breaker&& breaker) = delete;
    Breaker operator = (const Breaker& breaker) = delete;

    std::vector<Modifier> read_modifiers(const std::string& string_modifiers) const;

    void clear_string(std::string& str) const;

    uint lines_in_block(const std::string& block) const;

    std::vector<std::string> read_args(std::string string_args) const;

    std::string method_to_string(const Method& method, bool more_info) const;


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