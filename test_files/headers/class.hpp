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

class AA {

};

class BB;

class Beautifier : public fs::path, protected std::string, 
std::vector<std::string> 
{

    public:
        Beautifier() = delete;
        Beautifier(const std::string& header, 
                const std::string& source);
        
        void reorder_source();
        void reorder_header();
        void beautify();

        std::string getHeader() const;
        std::string getSource() const;

protected:
    enum class AccessSpecifier {
        PUBLIC,
        PRIVATE,
        PROTECTED
    };
    struct Class {
        std::string name;
        std::vector<Class> inheritance_classes;
    };
    struct Include {
        std::string name;
        bool is_system;
    };
    struct Constructor {
        std::string name;
        std::string body;
        AccessSpecifier access;
    };
    struct Destructor {
        std::string body;
        AccessSpecifier access;
    };
    struct Member {
        std::string name;
        std::string body;
        AccessSpecifier access;
    };
    struct Method {
        std::string name;
        std::string body;
        AccessSpecifier access;
    };

protected:
    const fs::path header;
    const fs::path source;

    std::vector<Include> includes;
    std::vector<std::string> other_lines_before_class;
    Class _class;
    std::vector<Constructor> _constructors;
    std::vector<Member> _members;
    std::vector<Method> _methods;
    Destructor destructor;
    std::vector<std::string> other_lines_after_class;


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

    void clean_string(std::string& str, bool clear_initial_spaces) const;
    
    bool is_a_include       (const std::string& line) const;
    bool is_a_constructor   (const std::string& line, std::string class_name) const;
    bool is_a_destructor    (const std::string& line, std::string class_name) const;
    inline virtual bool is_a_method        (const std::string& line, std::string class_name) const;
    bool is_a_member        (const std::string& line, std::string class_name) const;

    void write_header(const std::vector<std::string>& lines) const;
    void write_source(const std::vector<std::string>& lines) const;
     
    
};

#endif // BEAUTIFIER_HPP