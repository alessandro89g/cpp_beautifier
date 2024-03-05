#include "../include/Beautifier.hpp"

#define DEBUG(x) std::cout << x << std::endl;

std::vector<std::string> split_string(const std::string& str, char delimiter = ',') {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(str);
    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}


Beautifier::Beautifier(const std::string& header, const std::string& source)
    : m_header(header), m_source(source), m_destructor("") {
    DEBUG("Parametric constructor")
    std::string class_inheritance = string_inheritance_and_access;
    DEBUG(class_inheritance)
}

std::string Beautifier::getHeader() const {
    return m_header.string();
}

std::string Beautifier::getSource() const {
    return m_source.string();
}

void Beautifier::reorder_header() {
    dissect_header();
}

void Beautifier::dissect_header() {
    DEBUG("Dissecting header")
    m_file.open(m_header);
    if (!m_file.is_open()) {
        throw std::runtime_error("File " + m_header.string() + " not found");
    }
    std::string line;
    while(std::getline(m_file, line)) {
        bool processed = false;
        if (line.find("#include") != std::string::npos) {
            processed = extract_include(line);
        }
        if (!processed && std::regex_match(line, std::regex("\\s*class\\s+.*\\{"))) {
            extract_class(line);
        }
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
        m_includes.emplace_back(include);
        return true;
    }
    DEBUG("No match found")
    return false;
}

bool Beautifier::extract_class(const std::string& line) {
    DEBUG("Checking if line is a class")
    DEBUG(line)
    std::string allowed_naming = "([a-zA-Z_][a-zA-Z0-9_]*)";
    std::string class_regex = "\\s*class\\s+" + allowed_naming;
    std::regex rgx(class_regex);
    std::smatch matches;
    if (std::regex_search(line, matches, rgx)) {
        DEBUG("Match found")
        for (size_t i = 0; i < matches.size(); ++i) {
            DEBUG(matches[i].str())
        }
        std::string class_name = matches[1].str();
        m_class.m_name = class_name;
        DEBUG("Class name: " + class_name)
    } else {
        return false;
    }

    rgx.assign("\\s*class\\s*([a-zA-Z_][a-zA-Z0-9_]*)\\s*:\\s*(.+)\\s\\{");
    DEBUG("CHECKING INHERITANCE")
    if (std::regex_match(line, matches, rgx)) {
        DEBUG("Match found")
        if (matches.size() != 3) {
            throw std::runtime_error("Error extracting inheritance");
        }
        std::string other = matches[2].str();
        DEBUG("Inheritance found: " + other)
        std::vector<std::string> inheritance = split_string(other);
        for (auto& i : inheritance) {
            clean_string(i, true);
            if (std::regex_match(i, matches, std::regex(string_inheritance_and_access))) {
                if (matches.size() != 14) {
                    throw std::runtime_error("Error extracting inheritance");
                }
                std::string access_specifier = matches[3].str();
                AccessSpecifier access;
                if (access_specifier == "public") {
                    access = AccessSpecifier::PUBLIC;
                } else if (access_specifier == "protected") {
                    access = AccessSpecifier::PROTECTED;
                } else {
                    access = AccessSpecifier::PRIVATE;
                }

                std::string class_name = matches[4].str();
                m_class.m_inheritance_classes.emplace_back(std::make_pair(class_name, access));
            }
            return true;
        }
    }
    DEBUG("No match found")
    return false;
}

bool Beautifier::is_a_constructor(const std::string& line) const {
    const std::string keywords = "\\s*(((static|virtual|explicit|inline|constexpr)\\s*)?)+";
    const std::string constructor_scheleton = 
            keywords + 
            "(" +
            m_class.m_name + 
            "::\\s*)?" + 
            m_class.m_name + 
            "\\s*(\\(|\\n).*";  // after constructor name there is usually something 
    return std::regex_match(line, std::regex(constructor_scheleton));
}

bool Beautifier::is_a_destructor(const std::string& line) const {
    const std::string destructor_scheleton =  
        "\\s*(" + 
        m_class.m_name +
        "::\\s*)?" +
        "~" + m_class.m_name +
        "\\s*\\(\\s*\\).*";
    return std::regex_match(line, std::regex(destructor_scheleton));
}

bool Beautifier::is_a_method(const std::string& line) const {
    const std::string keywords = "\\s*(?:(?:virtual|inline|explicit|constexpr|static|friend|override)\\s+)?";
    const std::string allowed_naming = "[a-zA-Z_][a-zA-Z0-9_]*";
    const std::string method_scheleton = 
        "(" +keywords +
        "((" + m_class.m_name + "::(?!" + m_class.m_name + ")" + allowed_naming + ")|" +
        "((?!" + m_class.m_name + ")" + allowed_naming + "))).*\\s*";   
    DEBUG(method_scheleton);
    DEBUG("---->" + line);
    return std::regex_match(line, std::regex(method_scheleton));
}

bool Beautifier::is_a_member(const std::string& line) const {
    return std::regex_match(line, std::regex(".*;"));
}

