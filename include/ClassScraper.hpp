#ifndef CLASS_SCRAPER
#define CLASS_SCRAPER
#include "FileReader.hpp"
#include "Breaker.hpp"

class ClassScraper : public FileReader, public Breaker {
public:
    ClassScraper() = delete;
    ClassScraper(std::ifstream& file);
    ClassScraper(const std::string& file_string);
    ClassScraper(const ClassScraper& class_scraper) = delete;
    ClassScraper(ClassScraper&& class_scraper) = delete;

    std::vector<std::string> get_methods();

    std::vector<std::string> get_classes();

protected:
    void scrape();
    
    void find_methods();

    void find_classes();
        
    std::vector<std::string> classes;
    std::vector<std::string> methods;
    std::vector<std::string> members;
    std::vector<std::string> namespaces;
    std::vector<un_ptr(Definition)> definitions; 

};

#endif // CLASS_SCRAPER_H