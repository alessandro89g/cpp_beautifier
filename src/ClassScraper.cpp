#include "../include/ClassScraper.hpp"

using namespace std;

ClassScraper::ClassScraper(const string& header, const string& source) : 
    _header(header), _source(source) 
{
    scrape();
}

vector<string> ClassScraper::get_methods() {
    return methods;
}

vector<string> ClassScraper::get_classes() {
    return classes;
}

string ClassScraper::get_header_content() const {
    return _header.header_reader->get_file_content();
}

string ClassScraper::get_source_content() const {
    return _source.source_reader->get_file_content();
}

void ClassScraper::scrape() {
    find_classes();
    find_methods();
}

void ClassScraper::find_methods() {
    const string pattern_string = METHOD_RGX;
    smatch match;
    regex pattern(pattern_string);
    string text = _header.header_reader->get_file_content();

    while(regex_search(text,match,pattern)) {
        methods.push_back(match.str());
        Method method = Breaker::get_instance().read_method(match.str(), 0, Access::PUBLIC);
        text = match.suffix();
    }
}

void ClassScraper::find_classes() {
    const string pattern_string = CLASS_NAME;
    smatch match;
    regex pattern(pattern_string);
    string text = _header.header_reader->get_file_content();

    while(regex_search(text,match,pattern)) {
        classes.push_back(match.str());
        text = match.suffix();
    }
}


queue<Breaker::Block> ClassScraper::break_into_blocks(const FileReader& file_reader) {
    return break_into_blocks(file_reader.get_file_content(true));
}


queue<Breaker::Block> ClassScraper::break_into_blocks(const string& content) {
    size_t brakets = 0;
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
        if (read_access(buffer).has_value()) {
            auto new_access = read_access(buffer);
            if (new_access.has_value())
                current_access_type = new_access.value();
            continue;
        }
        blocks.emplace(buffer, block_start, current_access_type);
    }
    return blocks;
}

Breaker::Type ClassScraper::get_type(const Block& block) const {
    Type type;
    // const string pattern_string = TYPE_RGX;
    // smatch match;
    // regex pattern(pattern_string);
    // string text = block.body;

    // uint line_start = block.line_start;
    

    // while(regex_search(text,match,pattern)) {
    //     types.push_back(Breaker::get_instance().read_type(match.str()));
    //     text = match.suffix();
    // }

    return type;
}