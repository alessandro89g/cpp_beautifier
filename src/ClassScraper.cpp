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
    string line;
    stringstream text(file_reader.get_file_content());
    size_t brakets = 0;
    uint line_index = 0;
    queue<Breaker::Block> blocks;
    while (getline(text, line, '\n')) {
        Block block("",line_index);
        if (line.find('{') != string::npos) {
            brakets++;
        }
        if (line.find('}') != string::npos) {
            brakets--;
        }
        block += line + '\n';
        if (!brakets) {
            blocks.push(std::move(block));
        }
    }

    return blocks;
}