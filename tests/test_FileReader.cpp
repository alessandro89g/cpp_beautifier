#include <gtest/gtest.h>
#include "../include/FileReader.hpp"

using namespace std;

const string HEADER = "test_files/include/class.hpp";
const string SOURCE = "test_files/src/class.cpp";

TEST(FileReader, GETHEADERCONTENT) {
    FileReader file_reader(HEADER);
    string header = file_reader.get_file_content(true);

    ifstream file(HEADER);
    string content = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    EXPECT_EQ(header, content);
}

TEST(FileReader, GETSOURCECONTENT) {
    FileReader file_reader(SOURCE);
    string source = file_reader.get_file_content(true);

    ifstream file(SOURCE);
    string content = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    EXPECT_EQ(source, content);
}

TEST(FileReader, EXPORTHEADER) {
    FileReader file_reader(HEADER);
    string header = file_reader.get_file_content(true);
    file_reader.export_file("test_files/include/exported_class.hpp", true);

    ifstream file("test_files/include/exported_class.hpp");
    string content = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    remove("test_files/include/exported_class.hpp");

    EXPECT_EQ(header, content);
}

TEST(FileReader, EXPORTSOURCE) {
    FileReader file_reader(SOURCE);
    string source = file_reader.get_file_content(true);
    file_reader.export_file("test_files/src/exported_class.cpp", true);

    ifstream file("test_files/src/exported_class.cpp");
    string content = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    remove("test_files/src/exported_class.cpp");

    EXPECT_EQ(source, content);
}