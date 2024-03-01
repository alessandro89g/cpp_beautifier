#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "include/Colorifier.hpp"

const std::string VERSION = "0.1.0";
const std::string AUTHOR = "Alessandro Grassi";
const std::string PROJECT = "cpp_beautifier";

#define COLORIFY(text, color) Colorifier::colorify(text, Colorifier::Color::color)

namespace fs = std::filesystem;

struct Files {
    std::string header;
    std::string source;
};

using std::cout;
using std::endl;
using std::cerr;

void print_usage() {
    cerr << 
        COLORIFY("Usage: \n->   ", RED) 
        << "./" << PROJECT << COLORIFY(" <header>", GREEN)
        << endl
        << COLORIFY("->   ", RED) 
        << "./" << PROJECT << COLORIFY(" <file_names_1> <file_names_2>", GREEN)
        << endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    Files files;

    if (argc == 2) {
        if (fs::path(argv[1]).extension() == ".hpp" || fs::path(argv[1]).extension() == ".h") {
            files.header = argv[1];
            files.source = fs::path(argv[1]).replace_extension(".cpp").string();
        } else {
            cerr << COLORIFY("Invalid file extension: ", RED)
                 << "Expected .hpp or .h\n";
            print_usage();
            return 1;
        }
    }
    if (argc == 3) {
        std::string file1 = argv[1];
        std::string file2 = argv[2];

        if (fs::path(file1).extension() == ".hpp" || fs::path(file1).extension() == ".h") {
            files.header = file1;
            files.source = file2;
        } else {
            files.header = file2;
            files.source = file1;
        }
    }
    
    cout << COLORIFY("Header: ", GREEN) << files.header << endl;
    cout << COLORIFY("Source: ", GREEN) << files.source << endl;

    // Check that both files exist
    if (!fs::exists(files.header)) {
        cerr << COLORIFY("File not found: ", RED) << files.header << endl;
        return 1;
    }
    if (!fs::exists(files.source)) {
        cerr << COLORIFY("File not found: ", RED) << files.source << endl;
        return 1;
    }
    
    

    return 0;
}