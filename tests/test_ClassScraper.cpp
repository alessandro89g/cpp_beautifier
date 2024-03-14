#include <gtest/gtest.h>
#include <fstream>
#include "../include/ClassScraper.hpp"

using namespace std;

#include "ClassScraper_test_utilities.hpp"

const string header_path = "test_files/include/class.hpp";
const string source_path = "test_files/src/class.cpp";

static ClassScraper class_scraper(header_path, source_path);

TEST(CLASS_SCRAPER_TEST, CONSTRUCTOR) {
    EXPECT_EQ(class_scraper.get_header_file_name(), header_path);
    EXPECT_EQ(class_scraper.get_source_file_name(), source_path);
}

TEST(CLASS_SCRAPER_TEST, NOT_EXISTING_FILE) {
    EXPECT_THROW(ClassScraper class_scraper("asd.hpp", "asd.cpp"), runtime_error);
}

TEST(CLASS_SCRAPER_TEST, GET_ORIGINAL_FILE_CONTENT_HEADER) {
    ifstream header(header_path);
    string file_content;
    if (header.is_open()) {
        string line;
        while(getline(header, line))
            file_content += line + '\n';
    }
    ASSERT_EQ(class_scraper.get_header_content(true).size(), file_content.size());
    EXPECT_EQ(class_scraper.get_header_content(true), file_content);
}

TEST(CLASS_SCRAPER_TEST, GET_ORIGINAL_FILE_CONTENT_SOURCE) {
    ifstream source(source_path);
    string file_content;
    if (source.is_open()) {
        string line;
        while(getline(source, line))
            file_content += line + '\n';
    }
    ASSERT_EQ(class_scraper.get_source_content(true).size(), file_content.size());
    EXPECT_EQ(class_scraper.get_source_content(true), file_content);
}

TEST(CLASS_SCRAPER_TEST, GET_HEADER_FILE_HEADERS) {
    const ClassScraper::HeaderFile& header = class_scraper.get_header();
    
    auto includes = header.get_includes();
    DEBUG(includes.size())
    for (size_t i=0; i<includes.size(); ++i)
        ASSERT_EQ(includes[i], Includes[i]);
}