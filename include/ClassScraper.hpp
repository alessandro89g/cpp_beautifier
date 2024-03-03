#ifndef CLASS_SCRAPER_HPP
#define CLASS_SCRAPER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <regex>
#include <memory>

using std::string;
using std::vector;
using std::ifstream;
using std::regex;
using std::pair;
#define sh_ptr std::shared_ptr

#define VALID_NAME "([a-zA-Z_][a-zA-Z0-9_]*)"
#define VALID_HEADER "((\\s*#include\\s+<" VALID_NAME ">)|(\\s*#include\\s+\"(" VALID_NAME "\\.[h|hpp])\"))" 
#define DEBUG(...) std::cout << __VA_ARGS__ << std::endl;



class ClassScraper {
public:
    ClassScraper(const string& header, const string& source);
    ClassScraper(const ClassScraper& other);
    ClassScraper(ClassScraper&& other);

    void dissect_header();
    
    class Block;
    class Element;
    class Class;
    class Include;
    class Constructor;
    class Destructor;
    class Member;
    class Method;

    vector<sh_ptr<Block>> get_blocks() const { return m_blocks; }

private:
    string m_header;
    string m_source;
    string m_namespace;
    string m_previous_namespace;
    uint m_depth;
    vector<sh_ptr<Block>> m_blocks;
};





class ClassScraper::Block {
public:
    Block(const vector<string>& body, uint first_line_number, string Namespace, uint base_depth) 
        : m_body(std::move(body)), m_first_line_number(first_line_number), m_namespace(Namespace), m_head(""), m_base_depth(base_depth) {
            m_last_line_number = m_first_line_number + m_body.size() - 1;
        }
    Block(const Block& other) : m_body(other.m_body), m_head(other.m_head), m_first_line_number(other.m_first_line_number), m_last_line_number(other.m_last_line_number), m_base_depth(other.m_base_depth), m_namespace(other.m_namespace) {}   
    Block(Block&& other) : m_body(std::move(other.m_body)), m_head(std::move(other.m_head)), m_first_line_number(other.m_first_line_number), m_last_line_number(other.m_last_line_number), m_base_depth(other.m_base_depth), m_namespace(std::move(other.m_namespace)) {}

    vector<string> get_body() const { return m_body; }
    string get_head() const { return m_head; }
    uint get_first_line_number() const { return m_first_line_number; }
    uint get_last_line_number() const { return m_last_line_number; }
    uint get_base_depth() const { return m_base_depth; }
    string get_namespace() const { return m_namespace; }

    virtual ~Block() = default;

protected:
    vector<string> m_body;
    string m_head;
    uint m_first_line_number;
    uint m_last_line_number;
    uint m_base_depth;
    string m_namespace;
};

class ClassScraper::Include : public ClassScraper::Block {
public:
    Include(const string& line, uint line_number, uint base_depth) : Block(vector<string>{line}, line_number, "", base_depth) {
        process();
    }
    Include(const Include& other) : Block(other), m_name(other.m_name), m_is_system(other.m_is_system) {}
    Include(Include&& other) : Block(std::move(other)), m_name(std::move(other.m_name)), m_is_system(other.m_is_system) {}

    string get_name() const { return m_name; }
    bool is_system() const { return m_is_system; }
    string reconstruct() const {
        string result;
        uint depth = Block::m_base_depth;
        while(depth--) {
            DEBUG("Depth: " << depth)
            result += "    ";
        }
        result += "#include ";
        (m_is_system) ? result += "<" : result += "\"";
        result += m_name;
        (m_is_system) ? result += ">" : result += "\"";
        return result;
    }

private:
    string m_name;
    bool m_is_system;

    void process() {
        regex r(VALID_HEADER);
        std::smatch match;
        if (regex_search(m_body[0], match, r)) {
            if (m_body[0].find("<") != string::npos) {
                m_is_system = true;
                m_name = match[3].str();
            } else {
                m_is_system = false;
                m_name = match[5].str();
            }
            std::cout.setf(std::ios::boolalpha);
        }
        if (m_body[0].find("<") != string::npos) {
            m_is_system = true;
        }
    }
};


class ClassScraper::Element : public ClassScraper::Block {
    Element(vector<string>& body, uint first_line_number, string Namespace, uint base_depth) 
        : Block(body, first_line_number, Namespace, base_depth) {}
protected:
    string m_name;
    string m_access;
    enum ElementType {
        CLASS,
        STRUCT,
        UNION,
        ENUM,
        NAMESPACE,
        METHOD,
        MEMBER,
        CONSTRUCTOR,
        DESTRUCTOR
    };
};


class ClassScraper::Class : public ClassScraper::Element {
};

class ClassScraper::Destructor : public ClassScraper::Element {
};

class ClassScraper::Member : public ClassScraper::Element {
};

class ClassScraper::Method : public ClassScraper::Element {
};

#endif // CLASS_SCRAPER_HPP