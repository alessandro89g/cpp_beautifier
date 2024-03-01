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
    bool is_a_destructor(const std::string& line) const {
        return Beautifier::is_a_destructor(line, "Automobile");
    }
    bool is_a_method(const std::string& line) const {
        return Beautifier::is_a_method(line, "Automobile");
    }
};

BeautifierTest beautifier(header, source);

TEST(BeautifierTest, ParametricConstructor) {
    EXPECT_EQ(beautifier.getHeader(), header);
    EXPECT_EQ(beautifier.getSource(), source);
}   

TEST(BeautifierTest, is_a_include) {
    EXPECT_TRUE(beautifier.is_a_include("#include <iostream>"));
    EXPECT_TRUE(beautifier.is_a_include("#include \"automobile.h\""));
    EXPECT_FALSE(beautifier.is_a_include("class Automobile {"));
    EXPECT_TRUE(beautifier.is_a_include("   #include <iostream>"));
    EXPECT_TRUE(beautifier.is_a_include("\t  #include \"automobile.h\""));
    EXPECT_FALSE(beautifier.is_a_include("#include asda"));
}

TEST(BeautifierTest, is_a_constructor) {
    EXPECT_TRUE(beautifier.is_a_constructor("Automobile::Automobile()"));
    EXPECT_TRUE(beautifier.is_a_constructor("Automobile::Automobile() {"));
    EXPECT_TRUE(beautifier.is_a_constructor("Automobile::Automobile(int a, int b) {"));
    EXPECT_TRUE(beautifier.is_a_constructor("Automobile::Automobile();"));
    EXPECT_TRUE(beautifier.is_a_constructor("Automobile::Automobile(int a, int b);"));
    EXPECT_TRUE(beautifier.is_a_constructor("Automobile::Automobile(int a, int b)"));
    EXPECT_TRUE(beautifier.is_a_constructor("static Automobile::Automobile()"));
    EXPECT_TRUE(beautifier.is_a_constructor("virtual Automobile::Automobile()"));
    EXPECT_TRUE(beautifier.is_a_constructor("constexpr explicit Automobile::Automobile()"));
    EXPECT_TRUE(beautifier.is_a_constructor("constexpr explicit Automobile::Automobile  \n"));
    EXPECT_FALSE(beautifier.is_a_constructor("Automobile::Automobil"));
    EXPECT_FALSE(beautifier.is_a_constructor("Autamobile::Automobile"));
    
}

TEST(BeautifierTest, is_a_destructor) {
    EXPECT_TRUE(beautifier.is_a_destructor("Automobile::~Automobile()"));
    EXPECT_TRUE(beautifier.is_a_destructor("Automobile::~Automobile() {"));
    EXPECT_TRUE(beautifier.is_a_destructor("Automobile::~Automobile();"));
    EXPECT_FALSE(beautifier.is_a_destructor("Automobile::~Automobil"));
    EXPECT_FALSE(beautifier.is_a_destructor("Autamobile::~Automobile"));
    EXPECT_TRUE(beautifier.is_a_destructor("Automobile::~Automobile  ()"));
    EXPECT_TRUE(beautifier.is_a_destructor("Automobile::  ~Automobile() {"));
}

TEST(BeautifierTest, is_a_method) {
    EXPECT_TRUE(beautifier.is_a_method("void Automobile::start()"));
    EXPECT_TRUE(beautifier.is_a_method("void Automobile::start() {"));
    EXPECT_TRUE(beautifier.is_a_method("void Automobile::start(int a, int b) {"));
    EXPECT_TRUE(beautifier.is_a_method("void Automobile::start();"));
    EXPECT_TRUE(beautifier.is_a_method("void Automobile::start(int a, int b);"));
    EXPECT_TRUE(beautifier.is_a_method("void Automobile::start(int a, int b)"));
    EXPECT_TRUE(beautifier.is_a_method("static void Automobile::start()"));
    EXPECT_TRUE(beautifier.is_a_method("virtual void Automobile::start()"));
    EXPECT_TRUE(beautifier.is_a_method("constexpr explicit void Automobile::start()"));
    EXPECT_TRUE(beautifier.is_a_method("constexpr explicit void Automobile::start  \n"));
    EXPECT_TRUE(beautifier.is_a_method("Automobile::start"));
    EXPECT_FALSE(beautifier.is_a_method("Autamobile::start"));
    EXPECT_FALSE(beautifier.is_a_method("void Automobile::1_cjoin()"));
}