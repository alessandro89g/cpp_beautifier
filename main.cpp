#include "include/ClassScraper.hpp"
#include "include/utilities.hpp"
#include <queue>

#ifndef DEBUG
    #define DEBUG(...)  cout << __VA_ARGS__ << endl;
#endif // DEBUG

using namespace std;



int main(int argc, char const *argv[]) {
    DEBUG("STARTED");
    if (argc != 3) {
        DEBUG("Usage: " << argv[0] << " <header_file> <source_file>")
        return 1;
    }
    ClassScraper class_scraper(argv[1], argv[2]);

    string header = class_scraper.get_header_content();
    DEBUG("Header: " << header)
    DEBUG("Lines: " <<  string_split(header).size())
    DEBUG("=====================================")

    queue<Breaker::Block> blocks = class_scraper.split_in_blocks(header);
    size_t block_number = blocks.size();
    while(!blocks.empty()) {
        DEBUG("=====================================")
        DEBUG("Block from line "<< blocks.front().line_start << " until line " << blocks.front().line_end << ": \n" << blocks.front().body)
        blocks.pop();
    }
    DEBUG("Number of blocks: " << block_number)
    DEBUG("=====================================")

    string source = class_scraper.get_source_content();
    DEBUG("Source: " << source)
    DEBUG("Lines: " <<  string_split(source).size())
    DEBUG("=====================================")

    blocks = class_scraper.split_in_blocks(source);
    block_number = blocks.size();
    while(!blocks.empty()) {
        DEBUG("=====================================")
        DEBUG("Block from line "<< blocks.front().line_start << " until line " << blocks.front().line_end << ": \n" << blocks.front().body)
        blocks.pop();
    }
    DEBUG("Number of blocks: " << block_number)
    DEBUG("=====================================")


    blocks = class_scraper.break_into_blocks(FileReader(argv[1]));
    block_number = blocks.size();
    while(!blocks.empty()) {
        DEBUG("=====================================")
        DEBUG("Block from line "<< blocks.front().line_start << " until line " << blocks.front().line_end << ": \n" << blocks.front().body)
        blocks.pop();
    }
    DEBUG("Number of blocks: " << block_number)
    DEBUG("=====================================")

    return 1;

    size_t pos = 0;
    for (const string& method : class_scraper.get_methods()) {
        pos++;
        DEBUG("Method " << pos << ": " << method)
    }

    pos = 0;
    for (const string& class_ : class_scraper.get_classes()) {
        pos++;
        DEBUG("Class " << pos << ": " << class_)
    }

    DEBUG("FINISHED");

    return 0;
}