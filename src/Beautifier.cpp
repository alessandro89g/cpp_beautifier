#include "../include/Beautifier.hpp"

Beautifier::Beautifier(const std::string& header, const std::string& source)
    : header(header), source(source) {
}

void Beautifier::reorder_header() {
    dissect_header();
}

void Beautifier::dissect_header() {
    includes = extract_includes();
    other_lines_before_class = extract_other_lines_before_class();
    class_name = extract_class_name();
    public_members = extract_public_members(); 
    private_members = extract_private_members();
    public_methods = extract_public_methods();
    private_methods = extract_private_methods();
    constructors = extract_constructors();
    destructor = extract_destructor();
    other_lines_after_class = extract_other_lines_after_class();
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

std::vector<std::string> Beautifier::extract_includes() const {
    std::vector<std::string> include_lines;
    std::ifstream file(header);
    std::string line;
    while (std::getline(file, line)) {
        if (std::regex_match(line, std::regex("^#include\\s+<.*>"))) {
            include_lines.push_back(line);
        }
    }
    return include_lines;
}

std::string Beautifier::extract_class_name() const {
    std::ifstream file(header);
    std::string line;
    while (std::getline(file, line)) {
        if (std::regex_match(line, std::regex("^class\\s+.*\\s*\\{"))) {
            clean_string(line, true);
            return line.substr(6, line.size() - 7);
        }
    }
    return "";
}

std::vector<std::string> Beautifier::extract_public_members() const {
    std::vector<std::string> public_members;
    std::ifstream file(header);
    std::string line;
    bool public_section = false;
    while (std::getline(file, line)) {
        if (std::regex_match(line, std::regex("^public:"))) {
            public_section = true;
            continue;
        }
        if (std::regex_match(line, std::regex("^private:"))) {
            public_section = false;
            continue;
        }
        if (public_section) {
            clean_string(line, true);
            public_members.push_back(line);
        }
    }
    return public_members;
}

std::vector<std::string> Beautifier::extract_private_members() const {
    std::vector<std::string> private_members;
    std::ifstream file(header);
    std::string line;
    bool private_section = false;
    while (std::getline(file, line)) {
        if (std::regex_match(line, std::regex("^private:"))) {
            private_section = true;
            continue;
        }
        if (private_section) {
            clean_string(line, true);
            private_members.push_back(line);
        }
    }
    return private_members;
}

std::vector<std::string> Beautifier::extract_public_methods() const {
    std::vector<std::string> public_methods;
    std::ifstream file(header);
    std::string line;
    bool public_section = false;
    while (std::getline(file, line)) {
        if (std::regex_match(line, std::regex("^public:"))) {
            public_section = true;
            continue;
        }
        if (std::regex_match(line, std::regex("^private:"))) {
            public_section = false;
            continue;
        }
        if (public_section) {
            clean_string(line, true);
            public_methods.push_back(line);
        }
    }
    return public_methods;
}

std::vector<std::string> Beautifier::extract_private_methods() const {
    std::vector<std::string> private_methods;
    std::ifstream file(header);
    std::string line;
    bool private_section = false;
    while (std::getline(file, line)) {
        if (std::regex_match(line, std::regex("^private:"))) {
            private_section = true;
            continue;
        }
        if (private_section) {
            clean_string(line, true);
            private_methods.push_back(line);
        }
    }
    return private_methods;
}

std::vector<std::string> Beautifier::extract_constructors() const {
    std::vector<std::string> constructors;
    std::ifstream file(header);
    std::string line;
    while (std::getline(file, line)) {
        if (std::regex_match(line, std::regex(".*\\(.*\\)\\s*\\{"))) {
            clean_string(line, true);
            constructors.push_back(line);
        }
    }
    return constructors;
}

std::string Beautifier::extract_destructor() const {
    std::ifstream file(header);
    std::string line;
    while (std::getline(file, line)) {
        if (std::regex_match(line, std::regex("^~.*\\(.*\\)\\s*\\{"))) {
            clean_string(line, true);
            return line;
        }
    }
    return "";
}

std::vector<std::string> Beautifier::extract_other_lines_after_class() const {
    std::vector<std::string> other_lines;
    std::ifstream file(header);
    std::string line;
    bool class_found = false;
    while (std::getline(file, line)) {
        if (std::regex_match(line, std::regex("^class\\s+.*\\s*\\{"))) {
            class_found = true;
            continue;
        }
        if (class_found) {
            clean_string(line, true);
            other_lines.push_back(line);
        }
    }
    return other_lines;
}

std::vector<std::string> Beautifier::extract_other_lines_before_class() const {
    std::vector<std::string> other_lines;
    std::ifstream file(header);
    std::string line;
    while (std::getline(file, line)) {
        if (std::regex_match(line, std::regex("^class\\s+.*\\s*\\{"))) {
            break;
        }
        clean_string(line, true);
        other_lines.push_back(line);
    }
    return other_lines;
}

bool Beautifier::is_a_method(const std::string& line) const {
    return std::regex_match(line, std::regex(".*\\(.*\\)\\s*\\{"));
}

bool Beautifier::is_a_member(const std::string& line) const {
    return std::regex_match(line, std::regex(".*;"));
}

bool Beautifier::is_a_constructor(const std::string& line) const {
    return std::regex_match(line, std::regex(".*\\(.*\\)\\s*\\{"));
}

bool Beautifier::is_a_destructor(const std::string& line) const {
    return std::regex_match(line, std::regex("^~.*\\(.*\\)\\s*\\{"));
}