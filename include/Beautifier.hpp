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
public:
    Beautifier() = delete;
    Beautifier(const std::string& header, const std::string& source);
    
    void reorder_source();
    void reorder_header();
    void beautify();

private:
    const fs::path header;
    const fs::path source;

    std::vector<std::string> includes;
    std::vector<std::string> other_lines_before_class;
    std::string class_name;
    std::vector<std::string> public_members;
    std::vector<std::string> private_members;
    std::vector<std::string> public_methods;
    std::vector<std::string> private_methods;
    std::vector<std::string> constructors;
    std::string destructor;
    std::vector<std::string> other_lines_after_class;


private:
    void dissect_header();
    void dissect_source();
    std::vector<std::string> extract_includes() const;
    std::string extract_class_name() const;
    std::vector<std::string> extract_public_members() const;
    std::vector<std::string> extract_private_members() const;
    std::vector<std::string> extract_public_methods() const;
    std::vector<std::string> extract_private_methods() const;
    std::vector<std::string> extract_constructors() const;
    std::string extract_destructor() const;
    std::vector<std::string> extract_other_lines_after_class() const;
    std::vector<std::string> extract_other_lines_before_class() const;

    void clean_string(std::string& str, bool clear_initial_spaces) const;
    bool is_a_method(const std::string& line) const;
    bool is_a_member(const std::string& line) const;
    bool is_a_constructor(const std::string& line) const;
    bool is_a_destructor(const std::string& line) const;

    void write_header(const std::vector<std::string>& lines) const;
    void write_source(const std::vector<std::string>& lines) const;
     
    
};

#endif // BEAUTIFIER_HPP