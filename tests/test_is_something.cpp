#include <gtest/gtest.h>
#include <iostream>

#include "../include/Beautifier.hpp"

const std::string header = "test_files/include/class.h";
const std::string source = "test_files/src/class.cpp";

class BeautifierTest : public Beautifier{
public:    
    BeautifierTest(const std::string& header, const std::string& source) : 
        Beautifier(header, source) {}
    bool is_a_include(const std::string& line) {
        return Beautifier::is_a_include(line);
    }
    bool is_a_constructor(const std::string& line) const {
        return Beautifier::is_a_constructor(line);
    }
    bool is_a_destructor(const std::string& line) const {
        return Beautifier::is_a_destructor(line);
    }
    bool is_a_method(const std::string& line) const {
        return Beautifier::is_a_method(line);
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

TEST(BeautifierTest, IsAInclude) {
    std::string line = "#include <iostream>";
    EXPECT_TRUE(beautifier.is_a_include(line));
    line = "#include \"class.h\"";
    EXPECT_TRUE(beautifier.is_a_include(line));
    line = "#include <class.h>";
    EXPECT_TRUE(beautifier.is_a_include(line));
    line = "#include <iostream";
    EXPECT_FALSE(beautifier.is_a_include(line));

    std::vector<std::pair<std::string, bool>> includes = beautifier.includes(); 
    EXPECT_EQ(includes.size(), 3);
    EXPECT_EQ(includes[0].first, "iostream");
    EXPECT_EQ(includes[0].second, true);
    EXPECT_EQ(includes[1].first, "class.h");
    EXPECT_EQ(includes[1].second, false);
    EXPECT_EQ(includes[2].first, "class.h");
    EXPECT_EQ(includes[2].second, true);

}