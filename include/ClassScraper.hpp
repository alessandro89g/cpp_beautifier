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

    std::string get_header_content() const;
    std::string get_source_content() const;

    struct Class : public Definition {
    private:
        std::vector<Include> includes;
        std::vector<Member> members;
        std::vector<Method> methods;
        std::vector<Constructor> constructors;
        Destructor destructors;
        std::vector<Class> nested_classes;
    };

    struct Line {
        std::string content;
        uint line_number;
        operator std::string () const { return content; }
    };

    struct HeaderFile {
        explicit HeaderFile(const std::string& name_with_path) 
            : name_with_path(name_with_path), header_reader(std::make_unique<FileReader>(name_with_path)) {}
        HeaderFile(HeaderFile&& header_file) :  name_with_path(header_file.name_with_path), header_reader(std::move(header_file.header_reader)),
            includes(std::move(header_file.includes)), classes(std::move(header_file.classes)), extra_lines(std::move(header_file.extra_lines)) {}
        std::string name_with_path;
        std::unique_ptr<FileReader> header_reader;
        std::vector<Include> includes;
        std::vector<Class> classes;
        std::vector<Line> extra_lines; 
    };

    struct SourceFile {
        explicit SourceFile(const std::string& name_with_path) 
            : name_with_path(name_with_path), source_reader(std::make_unique<FileReader>(name_with_path)) {}
        SourceFile(SourceFile&& source_file) :  name_with_path(source_file.name_with_path), source_reader(std::move(source_file.source_reader)),
                                                includes(std::move(source_file.includes)), methods(std::move(source_file.methods)), extra_lines(std::move(source_file.extra_lines)) {}
        std::string name_with_path;
        std::unique_ptr<FileReader> source_reader;
        std::vector<Include> includes;
        std::vector<Method> methods;
        std::vector<Line> extra_lines; 
    };

protected:
    void scrape();
    
    void find_methods();

    void find_classes();
        
    std::vector<std::string> classes;
    std::vector<std::string> methods;
    std::vector<std::string> members;
    std::vector<std::string> namespaces;
    HeaderFile header;
    SourceFile source;
};

#endif // CLASS_SCRAPER_H   