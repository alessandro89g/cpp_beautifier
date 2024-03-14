#include "../include/ClassScraper.hpp"

using namespace std;

ClassScraper::ClassScraper(const string& header, const string& source) : 
    _includes(), _extra_lines(), _methods(), _classes(),
    _header(header, _includes, _extra_lines, _methods, _classes), 
    _source(source, _includes, _extra_lines, _methods) 
{
    scrape();
}

vector<string> ClassScraper::get_methods() {
    return methods;
}

vector<string> ClassScraper::get_classes() {
    return classes;
}

string ClassScraper::get_header_content(bool original) const {
    return _header.get_file_content(original);
}

string ClassScraper::get_source_content(bool original) const {
    return _source.get_file_content(original);
}

const ClassScraper::HeaderFile& ClassScraper::get_header() const {
    return _header;
}

const ClassScraper::SourceFile& ClassScraper::get_source() const {
    return _source;
}

ClassScraper::Class ClassScraper::get_class() const {
    return _class;
}

void ClassScraper::scrape() {
    find_classes();
    find_methods();
    read_and_parse_header();
}

void ClassScraper::find_methods() {
    const string pattern_string = METHOD_RGX;
    smatch match;
    regex pattern(pattern_string);
    string text = _header.get_file_content();

    while(regex_search(text,match,pattern)) {
        methods.push_back(match.str());
        Method method = Breaker::get_instance().extract_method(match.str(), 0, Access::PUBLIC);
        text = match.suffix();
    }
}

void ClassScraper::find_classes() {
    const string pattern_string = CLASS_NAME;
    smatch match;
    regex pattern(pattern_string);
    string text = _header.get_file_content();

    while(regex_search(text,match,pattern)) {
        classes.push_back(match.str());
        text = match.suffix();
    }
}


queue<Breaker::Block> ClassScraper::break_into_blocks(const FileReader& file_reader) {
    return break_into_blocks(file_reader.get_file_content(true));
}


queue<Breaker::Block> ClassScraper::break_into_blocks(const string& content) {
    queue<Breaker::Block> blocks;

    uint block_start = 0;
    Access current_access_type;

    vector<string> lines = string_split(content);

    for (size_t line_index = 0; line_index < lines.size(); line_index++) {
        block_start = line_index + 1;
        string buffer = lines[line_index];
        size_t brakets = 0;

        if (remove_leading_trailing_spaces(buffer) == "" && brakets == 0) {
            continue;
        }
        
        bool starts_w_hash = starts_with(buffer, "#");
        bool starts_w_line_comment = starts_with(buffer, "//");
        bool starts_w_block_comment = starts_with(buffer, "/*");
        auto ends_w_semicol = [&buffer] () ->bool {
            return ends_with(buffer, ";");
        };
        auto ends_w_braket = [&buffer] () ->bool {
            return ends_with(buffer, "{");
        };

        if (starts_w_hash && brakets == 0) {
            buffer = remove_leading_trailing_spaces(buffer);
            blocks.emplace(buffer, block_start, Access::PUBLIC);
            continue;
        }

        if (starts_w_line_comment && brakets == 0) {
            blocks.emplace(buffer, block_start, Access::PUBLIC);
            continue;
        }
        if (starts_w_block_comment && brakets == 0) {
            while (lines[++line_index].find("*/") == string::npos) {
                buffer += '\n' + lines[line_index];
            }
            buffer += '\n' + lines[line_index];
            blocks.emplace(buffer, block_start, Access::PUBLIC);
            continue;
        }

        while(!ends_w_semicol() && !ends_w_braket()) {
            buffer += '\n' + lines[++line_index];
        }

        if (parentheses_balance(buffer, '{') != 0) {
            while (parentheses_balance(buffer, '{') != 0) {
                buffer += '\n' + lines[++line_index];
            }
            blocks.emplace(buffer, block_start, Access::PUBLIC);
            continue;
        }
        if (extract_access(buffer).has_value()) {
            auto new_access = extract_access(buffer);
            if (new_access.has_value())
                current_access_type = new_access.value();
            continue;
        }
        blocks.emplace(buffer, block_start, current_access_type);
    }
    return blocks;
}

void ClassScraper::read_and_parse_header() {
    queue<Block> header_blocks 
        = break_into_blocks(_header.get_file_content(true));
    // queue<Block> source_blocks = break_into_blocks(_source.source_reader->get_file_content());

    while (header_blocks.size()) {
        Block block = header_blocks.front();
        Type type = get_block_type(block);

        switch (type) {
            case Type::COMMENT: {
                uint line_start = block.line_start;
                for (const string& s : string_split(block.body,"\n")) {
                    Line line = Line(s, line_start++);
                    _extra_lines.push_back(move(line));
                }
                break;
            }
            case Type::INCLUDE: {
                Include include = extract_include(block); 
                _includes.push_back(move(include));
                break;
            }
            case Type::CLASS: {
                Class class_ = extract_class(block, Access::PUBLIC);
                _classes.push_back(move(class_));
                break;
            }
            case Type::METHOD: {
                Method method = extract_method(block, Access::PUBLIC);
                _methods.push_back(move(method));
                break;
            }
            default: {
                Line line = Line(block.body, block.line_start);
                _extra_lines.push_back(move(line));
                break;
            }
        }
        header_blocks.pop();
    }
}

Breaker::Type ClassScraper::get_block_type(const Block& block) const {
    if (block_is_comment(block)) {
        return Type::COMMENT;
    }
    if (block_is_include(block)) {
        return Type::INCLUDE;
    }
    if (block_is_class(block)) {
        return Type::CLASS;
    }
    if (block_is_method(block)) {
        return Type::METHOD;
    }
    return Type::OTHER;    
}

bool ClassScraper::block_is_comment(const Block& block) const {
    return starts_with(block.body, "//") || starts_with(block.body, "/*");
}

bool ClassScraper::block_is_include(const Block& block) const {
    return starts_with(block.body, "#include");
}

bool ClassScraper::block_is_class(const Block& block) const {
    return starts_with(block.body, "class");
}

bool ClassScraper::block_is_method(const Block& block) const {
    return regex_match(block.body, regex(METHOD_RGX));
}