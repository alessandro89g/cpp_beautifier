#include "../include/Beautifier.hpp"

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
    DEBUG("Dissecting header")
    _file.open(header);
    if (!_file.is_open()) {
        throw std::runtime_error("File " + header.string() + " not found");
    }
    std::string line;
    while(std::getline(_file, line)) {
        if (line.find("#include") != std::string::npos)
            extract_include(line);
    }
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

bool Beautifier::extract_include(const std::string& line) {
    DEBUG("Checking if line is an include")
    DEBUG(line)
    std::string header_regex = "\\s*#include\\s+(:?<(.*)>|\"(.*)\")";
    std::regex rgx(header_regex);
    std::smatch matches;
    if (std::regex_search(line, matches, rgx)) {
        DEBUG("Match found")
        for (size_t i = 0; i < matches.size(); ++i) {
            DEBUG(matches[i].str())
        }
        bool is_system = matches[1].str() == "<" + matches[2].str() + ">";
        std::string include_name = is_system ? matches[2].str() : matches[3].str();
        Include include(include_name, is_system);
        _includes.emplace_back(include);
        return true;
    }
    DEBUG("No match found")
    return false;
}

bool Beautifier::is_a_constructor(const std::string& line) const {
    const std::string keywords = "\\s*(((static|virtual|explicit|inline|constexpr)\\s*)?)+";
    const std::string constructor_scheleton = 
            keywords + 
            "(" +
            _class.name + 
            "::\\s*)?" + 
            _class.name + 
            "\\s*(\\(|\\n).*";  // after constructor name there is usually something 
    return std::regex_match(line, std::regex(constructor_scheleton));
}

bool Beautifier::is_a_destructor(const std::string& line) const {
    const std::string destructor_scheleton =  
        "\\s*(" + 
        _class.name +
        "::\\s*)?" +
        "~" + _class.name +
        "\\s*\\(\\s*\\).*";
    return std::regex_match(line, std::regex(destructor_scheleton));
}

bool Beautifier::is_a_method(const std::string& line) const {
    const std::string keywords = "\\s*(?:(?:virtual|inline|explicit|constexpr|static|friend|override)\\s+)?";
    const std::string allowed_naming = "[a-zA-Z_][a-zA-Z0-9_]*";
    const std::string method_scheleton = 
        "(" +keywords +
        "((" + _class.name + "::(?!" + _class.name + ")" + allowed_naming + ")|" +
        "((?!" + _class.name + ")" + allowed_naming + "))).*\\s*";   
    DEBUG(method_scheleton);
    DEBUG("---->" + line);
    return std::regex_match(line, std::regex(method_scheleton));
}

bool Beautifier::is_a_member(const std::string& line) const {
    return std::regex_match(line, std::regex(".*;"));
}

