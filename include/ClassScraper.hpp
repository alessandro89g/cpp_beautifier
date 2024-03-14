#ifndef CLASS_SCRAPER
#define CLASS_SCRAPER
#include "FileReader.hpp"
#include "Breaker.hpp"
#include <queue>
#include <vector>
#include <string>


class ClassScraper : public Breaker {
public:
    ClassScraper() = delete;
    ClassScraper(const std::string& header, const std::string& source);
    ClassScraper(const ClassScraper& class_scraper) = delete;
    ClassScraper(ClassScraper&& class_scraper) = delete;

    std::vector<std::string> get_methods();
    std::vector<std::string> get_classes();

    std::string get_header_content(bool original = false) const;
    std::string get_source_content(bool original = false) const;

    struct Line {
        Line(const std::string& content, uint line_number) : content(content), line_number(line_number) {}
        bool operator == (const Line& other) const {
            return content == other.content && line_number == other.line_number;
        }
        std::string content;
        uint line_number;
        operator std::string () const { return content; }
    };


    class CPP_File {
    public:
        CPP_File() = delete;
        explicit CPP_File(const std::string& file, std::vector<Include>& includes, std::vector<Line>& lines, 
            std::vector<Method>& methods) : _name_with_path(file), _reader(std::make_unique<FileReader>(file)),
            _includes(includes), _extra_lines(lines), _methods(methods) {}
        CPP_File(const CPP_File& ) = delete;
        CPP_File(CPP_File&& ) = delete;

        std::string get_file_content(bool original = false) const {
            return _reader->get_file_content(original);
        }
        std::string get_file_name() const {
            return _name_with_path;
        }
        std::vector<Include> get_includes() const {
            return _includes;
        }

    protected:
        std::string _name_with_path;
        std::unique_ptr<FileReader> _reader;
        std::vector<Include>& _includes;
        std::vector<Line>& _extra_lines; 
        std::vector<Method>& _methods;
    };

    class HeaderFile : public CPP_File {
    public:
        explicit HeaderFile(const std::string& name_with_path, std::vector<Include>& includes, std::vector<Line>& lines, 
            std::vector<Method>& methods, std::vector<Class>& classes) 
            : CPP_File(name_with_path, includes, lines, methods), _classes(classes) {}
        
        HeaderFile(const HeaderFile& ) = delete;
        HeaderFile(HeaderFile&& )  = delete;

    private:
        std::vector<Class>& _classes;
    };

    class SourceFile : public CPP_File {
    public:
        explicit SourceFile(const std::string& name_with_path, std::vector<Include>& includes, std::vector<Line>& lines, 
            std::vector<Method>& methods) 
            : CPP_File(name_with_path, includes, lines, methods) {}
        
        SourceFile(const SourceFile& ) = delete;
        SourceFile(SourceFile&& ) = delete;
    };

    std::string get_header_file_name() const {
        return _header.get_file_name();
    }
    std::string get_source_file_name() const {
        return _source.get_file_name();
    }
    
    
    Class get_class() const;
    const HeaderFile& get_header() const;
    const SourceFile& get_source() const;


protected:
    void scrape();
    std::queue<Breaker::Block> break_into_blocks(const std::string& content);
    std::queue<Breaker::Block> break_into_blocks(const FileReader& file_reader);    
    void read_and_parse_header();

    Breaker::Type get_block_type(const Block& block) const;
    bool block_is_comment       (const Block& block) const;
    bool block_is_include       (const Block& block) const;
    bool block_is_class         (const Block& block) const;
    bool block_is_method        (const Block& block) const;
    
    void find_methods();
    void find_classes();
        
    std::vector<std::string> classes;
    std::vector<std::string> methods;
    std::vector<std::string> members;
    std::vector<std::string> namespaces;

    std::vector<Include> _includes;
    std::vector<Line> _extra_lines;
    std::vector<Method> _methods;
    std::vector<Class> _classes;

    HeaderFile _header;
    SourceFile _source;
    Class _class;
};

#endif // CLASS_SCRAPER_H   