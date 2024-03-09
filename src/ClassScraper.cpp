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
    size_t brakets = 0;
    queue<Breaker::Block> blocks;
    string content = file_reader.get_file_content();

    uint line_index = 0;
    for (size_t i = 0; i < content.size(); i++) {
        vector<char> buffer;
        buffer.reserve(200);
        while(content[i] != ';') {
            buffer.push_back(content[i]);
            i++;
            if (content[i] == '\n')
                line_index++;
        }
        buffer.clear();
        blocks.emplace(string(buffer.begin(), buffer.end()), line_index);
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