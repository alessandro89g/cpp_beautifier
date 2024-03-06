#include "include/ClassScraper.hpp"

#ifndef DEBUG
    #define DEBUG(...)  cout << __VA_ARGS__ << endl;
#endif // DEBUG

using namespace std;



int main(int argc, char const *argv[]) {
    DEBUG("STARTED");
    ClassScraper file_reader(argv[1], argv[2]);

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