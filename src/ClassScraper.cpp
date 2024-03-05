#include "../include/ClassScraper.hpp"

using namespace std;

ClassScraper::ClassScraper(const string& header, const string& source) 
        : headed_reader(std::make_unique<FileReader>(header)),
          source_reader(std::make_unique<FileReader>(source))
{
    scrape();
}

vector<string> ClassScraper::get_methods() {
    return methods;
}

vector<string> ClassScraper::get_classes() {
    return classes;
}

void ClassScraper::scrape() {
    find_classes();
    find_methods();
}

void ClassScraper::find_methods() {
    const string pattern_string = METHOD_RGX;
    smatch match;
    regex pattern(pattern_string);
    string text = headed_reader->get_file_content();

    while(regex_search(text,match,pattern)) {
        methods.push_back(match.str());
        Method method = read_method(match.str(), 0, Access::PUBLIC);
        text = match.suffix();
    }
}

void ClassScraper::find_classes() {
    const string pattern_string = CLASS_NAME;
    smatch match;
    regex pattern(pattern_string);
    string text = headed_reader->get_file_content();

    while(regex_search(text,match,pattern)) {
        classes.push_back(match.str());
        text = match.suffix();
    }
}