#ifndef BEAUTIFIER_HPP
#define BEAUTIFIER_HPP
#include <string>
#include <filesystem>
#include <regex>
#include <fstream>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

class Beautifier {
    #define vector(a) std::vector<a>
    using string = std::string;

    public:
        Beautifier() = delete;
        Beautifier(const string& header, const string& source);
        
        void reorder_source();
        void reorder_header();
        void beautify();

        string getHeader() const;
        string getSource() const;

protected:
    enum class AccessSpecifier {
        PUBLIC,
        PRIVATE,
        PROTECTED
    };
    struct Class {
        string name;
        vector(Class) inheritance_classes;
        AccessSpecifier access; 
    };
    struct Include {
        string name;
        bool is_system;
        Include(const string& name, bool is_system) : name(name), is_system(is_system) {}
        Include(const Include& other) : name(other.name), is_system(other.is_system) {}
        Include(Include&& other) : name(std::move(other.name)), is_system(other.is_system) {}
    };
    struct Constructor {
        string name;
        string body;
        AccessSpecifier access;
        Constructor(const string& name, const string& body, AccessSpecifier access) : name(name), body(body), access(access) {}
        Constructor(const Constructor& other) : name(other.name), body(other.body), access(other.access) {}
        Constructor(Constructor&& other) : name(std::move(other.name)), body(std::move(other.body)), access(other.access) {}
    };
    struct Destructor {
        string body;
        AccessSpecifier access;
        Destructor(const string& body, AccessSpecifier access) : body(body), access(access) {}
        Destructor(const Destructor& other) : body(other.body), access(other.access) {}
        Destructor(Destructor&& other) : body(std::move(other.body)), access(other.access) {}
    };
    struct Member {
        string name;
        string body;
        AccessSpecifier access;
        Member(const string& name, const string& body, AccessSpecifier access) : name(name), body(body), access(access) {}
        Member(const Member& other) : name(other.name), body(other.body), access(other.access) {}
        Member(Member&& other) : name(std::move(other.name)), body(std::move(other.body)), access(other.access) {}
    };
    struct Method {
        string name;
        string body;
        AccessSpecifier access;
        Method(const string& name, const string& body, AccessSpecifier access) : name(name), body(body), access(access) {}
        Method(const Method& other) : name(other.name), body(other.body), access(other.access) {}
        Method(Method&& other) : name(std::move(other.name)), body(std::move(other.body)), access(other.access) {}
    };

protected:
    const fs::path header;
    const fs::path source;
    std::ifstream _file;

    vector(Include) _includes;
    vector(string) _other_before_class;
    Class _class;
    vector(Constructor) _constructors;
    vector(Member) _members;
    vector(Method) _methods;
    Destructor _destructor;
    vector(string) _other_after_class;


protected:
    void dissect_header();
    void dissect_source();
    
    void extract_includes();
    void extract_other_lines_before_class();
    void extract_class();
    void extract_constructors();
    void extract_destructor();
    void extract_members();
    void extract_methods();
    void extract_other_lines_after_class();

    void clean_string(string& str, bool clear_initial_spaces) const;
    
    bool is_a_include       (const string& line);
    bool is_a_constructor   (const string& line) const;
    bool is_a_destructor    (const string& line) const;
    bool is_a_method        (const string& line) const;
    bool is_a_member        (const string& line) const;

    void write_header(const vector(string)& lines) const;
    void write_source(const vector(string)& lines) const;
     
    
};

#endif // BEAUTIFIER_HPP