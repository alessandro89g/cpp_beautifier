#include "../include/ClassScraper.hpp"
#include "../include/utilities.hpp"

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
        Method method (std::move(Breaker::get_instance().read_method(match.str(), 0, Access::PUBLIC)));
        cout << method.name << endl;    
        cout << method.owner_class << endl;
        cout << method.return_type << endl;
        cout << method.args.size() << endl;
        for (const string& arg : method.args) {
            cout << arg << endl;
        }
        cout << method.pre_modifiers.size() << endl;
        for (const Modifier& mod : method.pre_modifiers) {
            cout << mod << endl;
        }
        cout << method.post_modifiers.size() << endl;
        for (const Modifier& mod : method.post_modifiers) {
            cout << mod << endl;
        }
        cout << method.definition_in_header << endl;
        cout << method.body.body << endl;
        cout << method.body.line_start << endl;
        cout << method.body.line_end << endl;
        cout << method.body.type << endl;

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
    string content = file_reader.get_file_content(true);

    uint line_index = 0;
    uint block_start = 0;
    Access current_access_type = Access::PUBLIC;
    bool is_hashtag = false;
    vector<char> buffer;
    buffer.reserve(200);
    for (size_t i = 0; i < content.size(); i++) {
        if (content[i] == '\n') {
            line_index++;
        }

        buffer.push_back(content[i]);
        if (content[i] == '#') {
            is_hashtag = true;
            string hashtag_line = read_till_eol(content, i);
            append_string_to_vec_chars(buffer, hashtag_line);
            Block block(string(buffer.begin(), buffer.end()), line_index);
            get_type(block);
        } // comtinue
    }
    return blocks;
}

Breaker::Type ClassScraper::get_type(const Block& block) const {
    Type type;
    smatch match;
    // regex pattern(INCLUDE); // continue

    return type;
}