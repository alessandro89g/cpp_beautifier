#ifndef CLASS_SCRAPER
#define CLASS_SCRAPER
#include "FileReader.hpp"
#include "Breaker.hpp"

class ClassScraper : public Breaker {
public:
    ClassScraper() = delete;
    ClassScraper(const std::string& header, const std::string& source);
    ClassScraper(const ClassScraper& class_scraper) = delete;
    ClassScraper(ClassScraper&& class_scraper) = delete;

    std::vector<std::string> get_methods();

    std::vector<std::string> get_classes();

    struct Class : Definition {
        std::vector<un_ptr(Include)> includes;
        std::vector<un_ptr(Member)> members;
        std::vector<un_ptr(Method)> methods;
        std::vector<un_ptr(Constructor)> constructors;
        Destructor destructors;
        std::vector<un_ptr(Class)> sub_classes;
    };

protected:
    void scrape();
    
    void find_methods();

    void find_classes();
        
    std::vector<std::string> classes;
    std::vector<std::string> methods;
    std::vector<std::string> members;
    std::vector<std::string> namespaces;
    std::vector<un_ptr(Definition)> definitions; 
    std::unique_ptr<FileReader> headed_reader;
    std::unique_ptr<FileReader> source_reader;

};

#endif // CLASS_SCRAPER_H