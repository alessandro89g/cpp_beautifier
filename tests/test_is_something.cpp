#include <gtest/gtest.h>
#include <iostream>

#include "../include/Beautifier.hpp"

const std::string header = "test_files/headers/class.hpp";
const std::string source = "test_files/src/class.cpp";

class BeautifierTest : public Beautifier{
public:    
    BeautifierTest(const std::string& header, const std::string& source) : 
        Beautifier(header, source) {}
    
    void dissect_header() {
        Beautifier::dissect_header();
    }

    std::vector<std::pair<std::string, bool>>  includes() const {
        std::vector<std::pair<std::string, bool>> includes;
        for (const auto& i : _includes) {
            includes.push_back(std::make_pair(i.name, i.is_system));
        }
        return includes;
    }
};

BeautifierTest beautifier(header, source);

TEST(BeautifierTest, ParametricConstructor) {
    EXPECT_EQ(beautifier.getHeader(), header);
    EXPECT_EQ(beautifier.getSource(), source);
}

TEST(BeautifierTest, dissect_header) {
    beautifier.dissect_header();
    #include <string>
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