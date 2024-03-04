#include "FileReader.h"

#define DEBUG(...)  cout << __VA_ARGS__ << endl;




int main(int argc, char const *argv[]) {
    FileReader file_reader(argv[1]);
    
    string text = file_reader.get_file_content();
    
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


    return 0;
}