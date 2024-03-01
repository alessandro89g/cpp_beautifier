#include "../headers/class.hpp"

#define DEBUG(x) std::cout << x << std::endl;

Beautifier::Beautifier(const std::string& header, const std::string& source)
    : header(header), source(source) {
}

std::string Beautifier::getHeader() const {
    return header.string();
}

std::string Beautifier::getSource() const {
    return source.string();
}

void Beautifier::reorder_header() {
    dissect_header();
}

void Beautifier::dissect_header() {
    // extract_includes();
    // extract_other_lines_before_class();
    // extract_class();
    // extract_destructor();
    // extract_members(); 
    // extract_methods();
    // extract_constructors();
    // extract_other_lines_after_class();
}

void Beautifier::clean_string(std::string& str, bool clear_initial_spaces = false) const {
    if (clear_initial_spaces) {
        str = std::regex_replace(str, std::regex("^\\s+"), "");
    }
    // clear trailing spaces or tabs
    str = std::regex_replace(str, std::regex("\\s+$"), "");
    str = std::regex_replace(str, std::regex("\\t+$"), ""); 
    // clear multiple spaces not at the beginning
        // To do
}

bool Beautifier::is_a_include(const std::string& line) const {
    return (
            std::regex_match(line, std::regex("\\s*#include\\s+<.*>"))
        ||  std::regex_match(line, std::regex("\\s*#include\\s+\".*\""))
    );
}

bool Beautifier::is_a_constructor(const std::string& line, std::string class_name = "") const {
    if (class_name == "")
        class_name = _class.name;
    const std::string keywords = "\\s*(((static|virtual|explicit|inline|constexpr)\\s*)?)+";
    const std::string constructor_scheleton = 
            keywords + 
            "(" +
            class_name + 
            "::\\s*)?" + 
            class_name + 
            "\\s*(\\(|\\n).*";  // after constructor name there is usually something 
    return std::regex_match(line, std::regex(constructor_scheleton));
}

bool Beautifier::is_a_destructor(const std::string& line, std::string class_name = "") const {
    if (class_name == "")
        class_name = _class.name;
    const std::string destructor_scheleton =  
        "\\s*(" + 
        class_name +
        "::\\s*)?" +
        "~" + class_name +
        "\\s*\\(\\s*\\).*";
    return std::regex_match(line, std::regex(destructor_scheleton));
}

bool Beautifier::is_a_method(const std::string& line, string class_name) const {
    if (class_name == "")
        class_name = _class.name;
    const std::string keywords = "\\s*(?:(?:virtual|inline|explicit|constexpr|static|friend|override)\\s+)?";
    const std::string allowed_naming = "[a-zA-Z_][a-zA-Z0-9_]*";
    const std::string method_scheleton = 
        "(" +keywords +
        "((" + class_name + "::(?!" + class_name + ")" + allowed_naming + ")|" +
        "((?!" + class_name + ")" + allowed_naming + "))).*\\s*";   
    DEBUG(method_scheleton);
    DEBUG("---->" + line);
    return std::regex_match(line, std::regex(method_scheleton));
}

bool Beautifier::is_a_member(const std::string& line, string class_name) const {
    if (class_name == "")
        class_name = _class.name;
    return std::regex_match(line, std::regex(".*;"));
}

