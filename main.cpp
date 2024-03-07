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
    ClassScraper file_reader(argv[1], argv[2]);

    string header = file_reader.get_header_content();
    DEBUG("Header: " << header)
    DEBUG("Lines: " <<  string_split(header).size())
    DEBUG("=====================================")

    queue<Breaker::Block> blocks = file_reader.split_in_blocks(header);
    size_t block_number = blocks.size();
    while(!blocks.empty()) {
        DEBUG("=====================================")
        DEBUG("Block from line "<< blocks.front().line_start << " until line " << blocks.front().line_end << ": \n" << blocks.front().body)
        blocks.pop();
    }
    DEBUG("Number of blocks: " << block_number)
    DEBUG("=====================================")

    string source = file_reader.get_source_content();
    DEBUG("Source: " << source)
    DEBUG("Lines: " <<  string_split(source).size())
    DEBUG("=====================================")

    blocks = file_reader.split_in_blocks(source);
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
    for (const string& method : file_reader.get_methods()) {
        pos++;
        DEBUG("Method " << pos << ": " << method)
    }

    pos = 0;
    for (const string& class_ : file_reader.get_classes()) {
        pos++;
        DEBUG("Class " << pos << ": " << class_)
    }

    DEBUG("FINISHED");

    return 0;
}