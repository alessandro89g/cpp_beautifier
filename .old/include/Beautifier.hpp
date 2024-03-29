#ifndef BEAUTIFIER_HPP
#define BEAUTIFIER_HPP
#include <string>
#include <filesystem>
#include <regex>
#include <fstream>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

#define variable_name  std::string("([a-zA-Z_][a-zA-Z0-9_]*)")
#define variable_name_with_namespace  std::string("((" + variable_name + "\\s*::\\s*)?" \
                                                    + variable_name + ")")

#define variable_name_with_template std::string("(" + variable_name_with_namespace + \
                                                "(<" + variable_name_with_namespace + ">)?)")

#define string_inheritance_and_access  std::string("(((public|private|protected)\\s*)?" \
                                            + variable_name_with_template + ")$")

class Beautifier {
    #define vector(a) std::vector<a>
    using string = std::string;

    public:
        Beautifier() = delete;
        Beautifier(const string& header, const string& source);
        
        enum class AccessSpecifier {
            PUBLIC,
            PROTECTED,
            PRIVATE
        };

        struct Class;
        struct Include;
        struct Constructor;
        struct Destructor;
        struct Member;
        struct Method;

        void reorder_source();
        void reorder_header();
        void beautify();

        string getHeader() const;
        string getSource() const;


public:
    struct Class {
        string m_name;
        string m_namespace;
        #define class_inherited std::pair<string, AccessSpecifier>
        vector(class_inherited) m_inheritance_classes;
    };
    struct Include {
        string name;
        bool is_system;
        Include(const string& name, bool is_system) : name(name), is_system(is_system) {}
        Include(const Include& other) : name(other.name), is_system(other.is_system) {}
        Include(Include&& other) : name(std::move(other.name)), is_system(other.is_system) {}
    };
    struct Constructor {
        string m_name;
        string m_body;
        string m_namespace;
        AccessSpecifier m_access;
        Constructor(const string& name, const string& body, const string& Namespace, AccessSpecifier access) : m_name(name), m_body(body), m_namespace(Namespace), m_access(access) {}
        Constructor(const Constructor& other) : m_name(other.m_name), m_body(other.m_body), m_namespace(other.m_namespace), m_access(other.m_access) {}
    };
    struct Destructor {
        string m_body;
        string m_namespace;
        AccessSpecifier m_access;
        Destructor(const string& Namespace, const string& body = "", AccessSpecifier access = AccessSpecifier::PUBLIC) : m_body(body), m_namespace(Namespace), m_access(access) {}
        Destructor(const Destructor& other) : m_body(other.m_body), m_namespace(other.m_namespace), m_access(other.m_access) {}
        Destructor(Destructor&& other) : m_body(std::move(other.m_body)), m_namespace(std::move(other.m_namespace)), m_access(other.m_access) {}
    };
    struct Member {
        string m_name;
        string m_body;
        string m_namespace;
        AccessSpecifier m_access;
        Member(const string& name, const string& body, const string& Namespace, AccessSpecifier access) : m_name(name), m_body(body), m_namespace(Namespace), m_access(access) {}
        Member(const Member& other) : m_name(other.m_name), m_body(other.m_body), m_namespace(other.m_namespace), m_access(other.m_access) {}
        Member(Member&& other) : m_name(std::move(other.m_name)), m_body(std::move(other.m_body)), m_namespace(std::move(other.m_namespace)), m_access(other.m_access) {}
    };
    struct Method {
        string m_name;
        string m_body;
        string m_namespace;
        AccessSpecifier m_access;
        Method(const string& name, const string& body,const string Namespace, AccessSpecifier access) : m_name(name), m_body(body), m_namespace(Namespace), m_access(access) {}
        Method(const Method& other) : m_name(other.m_name), m_body(other.m_body), m_namespace(other.m_namespace), m_access(other.m_access) {}
        Method(Method&& other) : m_name(std::move(other.m_name)), m_body(std::move(other.m_body)), m_namespace(std::move(other.m_namespace)), m_access(other.m_access) {}
    };

protected:
    const fs::path m_header;
    const fs::path m_source;
    std::ifstream m_file;

    vector(Include) m_includes;
    vector(string) m_other_before_class;
    Class m_class;
    vector(Constructor) m_constructors;
    vector(Member) m_members;
    vector(Method) m_methods;
    Destructor m_destructor;
    vector(string) m_other_after_class;

    vector(string) m_namespaces;


protected:
    void dissect_header();
    void dissect_source();
    
    bool extract_include(const string& line);
    void extract_other_lines_before_class();
    bool extract_class(const string& line);
    void extract_constructors();
    void extract_destructor();
    void extract_members();
    void extract_methods();
    void extract_other_lines_after_class();

    void clean_string(string& str, bool clear_initial_spaces) const;
    
    bool is_a_constructor   (const string& line) const;
    bool is_a_destructor    (const string& line) const;
    bool is_a_method        (const string& line) const;
    bool is_a_member        (const string& line) const;

    void write_header(const vector(string)& lines) const;
    void write_source(const vector(string)& lines) const;
     
    
};

#endif // BEAUTIFIER_HPP