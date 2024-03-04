#ifndef CLASS_SCRAPER_H
#define CLASS_SCRAPER_H

#include "FileReader.h"
#include "Breaker.h"

class ClassScraper : public FileReader, public Breaker {
public:
    ClassScraper() = delete;
    ClassScraper(ifstream& file) : FileReader(file) {
        scrape();
    }
    ClassScraper(const string& file_string) : FileReader(file_string) {
        scrape();
    }
    ClassScraper(const ClassScraper& class_scraper) = delete;
    ClassScraper(ClassScraper&& class_scraper) = delete;

    vector<string> get_methods() {
        return methods;
    }

    vector<string> get_classes() {
        return classes;
    }



protected:
    void scrape() {
        find_classes();
        find_methods();
    }
    void find_methods() {
        const string pattern_string = METHOD_RGX;
        smatch match;
        regex pattern(pattern_string);
        string text = file_content;

        while(regex_search(text,match,pattern)) {
            methods.push_back(match.str());
            Method method = read_method(match.str(), 0, 0, Access::PUBLIC);
            text = match.suffix();
        }
    }

    void find_classes() {
        const string pattern_string = CLASS_NAME;
        smatch match;
        regex pattern(pattern_string);
        string text = file_content;

        while(regex_search(text,match,pattern)) {
            classes.push_back(match.str());
            text = match.suffix();
        }
    }
    
    

    
    vector<string> classes;
    vector<string> methods;
    vector<string> members;
    vector<string> namespaces;
    vector<un_ptr(Definition)> definitions; 

};

#endif // CLASS_SCRAPER_H