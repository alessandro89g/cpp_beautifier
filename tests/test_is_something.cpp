#include <gtest/gtest.h>
#include <iostream>

#include "../include/Beautifier.hpp"

const std::string header = "test_files/include/automobile.h";
const std::string source = "test_files/src/automobile.cpp";

class BeautifierTest : public Beautifier{
public:    
    BeautifierTest(const std::string& header, const std::string& source) : 
        Beautifier(header, source) {}
    bool is_a_include(const std::string& line) const {
        return Beautifier::is_a_include(line);
    }
    bool is_a_constructor(const std::string& line) const {
        return Beautifier::is_a_constructor(line, "Automobile");
    }
};

BeautifierTest beautifier(header, source);

TEST(BeautifierTest, ParametricConstructor) {
    ASSERT_EQ(beautifier.getHeader(), header);
    ASSERT_EQ(beautifier.getSource(), source);
}   

TEST(BeautifierTest, is_a_include) {
    ASSERT_TRUE(beautifier.is_a_include("#include <iostream>"));
    ASSERT_TRUE(beautifier.is_a_include("#include \"automobile.h\""));
    ASSERT_FALSE(beautifier.is_a_include("class Automobile {"));
    ASSERT_TRUE(beautifier.is_a_include("   #include <iostream>"));
    ASSERT_TRUE(beautifier.is_a_include("\t  #include \"automobile.h\""));
    ASSERT_FALSE(beautifier.is_a_include("#include asda"));
}

TEST(BeautifierTest, is_a_constructor) {
    std::string class_name = "Automobile";
    ASSERT_TRUE(beautifier.is_a_constructor("Automobile::Automobile()"));
    ASSERT_TRUE(beautifier.is_a_constructor("Automobile::Automobile() {"));
    ASSERT_TRUE(beautifier.is_a_constructor("Automobile::Automobile(int a, int b) {"));
    ASSERT_TRUE(beautifier.is_a_constructor("Automobile::Automobile();"));
    ASSERT_TRUE(beautifier.is_a_constructor("Automobile::Automobile(int a, int b);"));
    ASSERT_TRUE(beautifier.is_a_constructor("Automobile::Automobile(int a, int b)"));
    ASSERT_FALSE(beautifier.is_a_constructor("Automobile::Automobil"));
    ASSERT_FALSE(beautifier.is_a_constructor("Autamobile::Automobile"));
    ASSERT_TRUE(beautifier.is_a_constructor("static Automobile::Automobile()"));
    ASSERT_TRUE(beautifier.is_a_constructor("virtual Automobile::Automobile()"));
    ASSERT_TRUE(beautifier.is_a_constructor("explicit Automobile::Automobile()"));
    
}