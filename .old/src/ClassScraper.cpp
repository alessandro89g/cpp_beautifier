#include "../include/ClassScraper.hpp"

// using namespace std;

#define VALID_NAME_WITH_NAMESPACE "(((" VALID_NAME "\\s*::\\s*)?" VALID_NAME "))"
#define VALID_NAME_WITH_TEMPLATE "(" VALID_NAME_WITH_NAMESPACE "(<" VALID_NAME_WITH_NAMESPACE ">)?)"
#define VALID_STRING_INHERITANCE_AND_ACCESS "(((public|private|protected)\\s*)?" VALID_NAME_WITH_TEMPLATE ")"

#define DEBUG(...) std::cout << __VA_ARGS__ << std::endl;

void clean_string(std::string& str, bool clear_initial_spaces = true) {
    if (clear_initial_spaces) {
        str = std::regex_replace(str, std::regex("^\\s+"), "");
    }
    // clear trailing spaces or tabs
    str = std::regex_replace(str, std::regex("\\s+$"), "");
    str = std::regex_replace(str, std::regex("\\t+$"), ""); 
    // clear multiple spaces not at the beginning
        // To do
}

ClassScraper::ClassScraper(const string& header, const string& source) : m_header(header), m_source(source) {
    dissect_header();
}

ClassScraper::ClassScraper(const ClassScraper& other) : m_header(other.m_header), m_source(other.m_source), m_namespace(other.m_namespace), m_previous_namespace(other.m_previous_namespace) {
    for (const auto& block : other.m_blocks) {
        sh_ptr<Block> new_block = std::make_shared<Block>(*block);
        m_blocks.emplace_back(std::move(new_block));
    }
}
ClassScraper::ClassScraper(ClassScraper&& other) : m_header(std::move(other.m_header)), m_source(std::move(other.m_source)), m_namespace(std::move(other.m_namespace)), m_previous_namespace(std::move(other.m_previous_namespace)) {
    for (auto& block : other.m_blocks) {
        m_blocks.push_back(std::move(block));
    }
}

void ClassScraper::dissect_header() {
    uint line_number = 0;
    m_namespace = m_previous_namespace = "";
    ifstream file(m_header);
    if (!file.is_open()) {
        throw std::runtime_error("File not found: " + m_header);
    }
    string line;
    while (getline(file, line)) {
        line_number++;
        if (std::regex_match(line, std::regex(COMMENT_LINE))) { // Replace std::match_regex with std::regex_match
            sh_ptr<Block> comment = std::make_shared<Block>(std::vector<string>{line}, line_number, "", 0);
//          DEBUG("Extracting comment: " << line)
            continue;
        }

        for (const auto& c : line) {
            if (c == '{') {
                m_depth++;
//              DEBUG("Depth: " << m_depth << " : changed at line " << line_number)
            }
            if (c == '}') {
                m_depth--;
//              DEBUG("Depth: " << m_depth << " : changed at line " << line_number)

            }
        }
        
        if (line.find("#include") != string::npos) {
            clean_string(line);
//          DEBUG("Extracting include: " << line)
            sh_ptr<Block> include = std::make_shared<Include>(line, line_number, m_depth);
            m_blocks.push_back(std::move(include));
            continue;
        }
    }
}