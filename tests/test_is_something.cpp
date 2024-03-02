#include <gtest/gtest.h>
#include <iostream>

#include "../include/Beautifier.hpp"

const std::string header = "test_files/headers/class.hpp";
const std::string source = "test_files/src/class.cpp";

class BeautifierTest : public Beautifier{
public:    
    BeautifierTest(const std::string& header, const std::string& source) : 
        Beautifier(header, source) {
            dissect_header();
        }

    std::vector<std::pair<std::string, bool>>  includes() const {
        std::vector<std::pair<std::string, bool>> includes;
        for (const auto& i : _includes) {
            includes.push_back(std::make_pair(i.name, i.is_system));
        }
        return includes;
    }
    Class get_class() const {
        return _class;
    }
};

BeautifierTest beautifier(header, source);

TEST(BeautifierTest, ParametricConstructor) {
    EXPECT_EQ(beautifier.getHeader(), header);
    EXPECT_EQ(beautifier.getSource(), source);
}

TEST(BeautifierTest, extract_include) {
// From the file: test_files/headers/class.hpp
// 
// #include <string>
// #include <filesystem>
// #include <regex>
// #include <fstream>
// #include <iostream>
// #include <vector>
    std::vector<std::pair<std::string, bool>> expected = {
        {"string", true},
        {"filesystem", true},
        {"regex", true},
        {"fstream", true},
        {"iostream", true},
        {"vector", true}
    };
    EXPECT_EQ(beautifier.includes(), expected);
}

TEST(BeautifierTest, extract_class) {
// From the file: test_files/headers/class.hpp
//
// class Beautifier : public fs::path, protected std::string, std::vector<std::string> 
    Beautifier::Class expected = {
        "Beautifier",
        {
            {"fs::path", Beautifier::AccessSpecifier::PUBLIC},
            {"std::string", Beautifier::AccessSpecifier::PROTECTED},
            {"std::vector<std::string>", Beautifier::AccessSpecifier::PRIVATE}
        }
    };
    Beautifier::Class beautifier_class = beautifier.get_class();
    ASSERT_EQ(beautifier_class.name, expected.name);
    for (size_t i = 0; i < expected.inheritance_classes.size(); ++i) {
        EXPECT_EQ(beautifier_class.inheritance_classes[i].first, expected.inheritance_classes[i].first);
        EXPECT_EQ(beautifier_class.inheritance_classes[i].second, expected.inheritance_classes[i].second);
    }
}