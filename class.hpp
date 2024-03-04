#ifndef BEAUTIFIER_HPP
#define BEAUTIFIER_HPP
#include <string>
#include <filesystem>
  #include <regex>
    #include <fstream>
#include <iostream>
#include <vector>

//#include "asd.h"

namespace fs = std::filesystem;

class Beautifier : public fs::path, protected std::string, 
std::vector<std::string> 
{
    #define vector(a) std::vector<a>
    using string = std::string;

    public:
        Beautifier() = delete;
        Beautifier(const string& header,  const string& source);
        
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
    };
    struct Include {
        string name;
        bool is_system;
    };
    struct Constructor {
        string name;
        string body;
        AccessSpecifier access;
    };
    struct Destructor {
        string body;
        AccessSpecifier access;
    };
    struct Member {
        string name;
        string body;
        AccessSpecifier access;
    };
    struct Method {
        string name;
        string body;
        AccessSpecifier access;
    };

protected:
    const fs::path header;
    const fs::path source;

    vector(Include) includes;
    vector(string) other_lines_before_class;
    Class _class;
    vector(Constructor) _constructors;
    vector(Member) _members;
    vector(Method) _methods;
    Destructor destructor;
    vector(string) other_lines_after_class;


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
    
    bool is_a_include       (const string& line) const;
    bool is_a_constructor   (const string& line, string class_name) const;
    bool is_a_destructor    (const string& line, string class_name) const;
    bool is_a_method        (const string& line, string class_name) const;
    bool is_a_member        (const string& line, string class_name) const;

    void write_header(const vector(string)& lines) const;
    void write_source(const vector(string)& lines) const;
     
    
};

#endif // BEAUTIFIER_HPP