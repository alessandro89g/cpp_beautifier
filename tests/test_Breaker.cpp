#include <gtest/gtest.h>
#include "../include/Breaker.hpp"

TEST(BreakerTest, ReadMethod) {
    Breaker& breaker = Breaker::get_instance();
    Breaker::Method method = breaker.extract_method("int main(int argc, char** argv) {", 0, Breaker::Access::PUBLIC);
    EXPECT_EQ(method.name, "main");
    EXPECT_EQ(method.return_type, "int");
    EXPECT_EQ(method.args.size(), 2);
    EXPECT_EQ(method.args[0], "int argc");
    EXPECT_EQ(method.args[1], "char** argv");
}

TEST(BreakerTest, ReadMethodWithFiveArguments) {
    Breaker& breaker = Breaker::get_instance();
    std::string code = "double calculate(int a, short b, int c, float d, double e) {\n"
                       "    double sum = a + b + c + d + e;\n"
                       "    double product = a * b * c * d * e;\n"
                       "    double difference = a - b - c - d - e;\n"
                       "    double quotient = a / b / c / d / e;\n"
                       "    double result = sum + product + difference + quotient;\n"
                       "    return result;\n"
                       "}";
    Breaker::Method method = breaker.extract_method(code, 0, Breaker::Access::PUBLIC);
    EXPECT_EQ(method.name, "calculate");
    EXPECT_EQ(method.return_type, "double");
    EXPECT_EQ(method.args.size(), 5);
    EXPECT_EQ(method.args[0], "int a");
    EXPECT_EQ(method.args[1], "short b");
    EXPECT_EQ(method.args[2], "int c");
    EXPECT_EQ(method.args[3], "float d");
    EXPECT_EQ(method.args[4], "double e");
    EXPECT_EQ(method.body.body, "    double sum = a + b + c + d + e;\n"
                           "    double product = a * b * c * d * e;\n"
                           "    double difference = a - b - c - d - e;\n"
                           "    double quotient = a / b / c / d / e;\n"
                           "    double result = sum + product + difference + quotient;\n"
                           "    return result;");
}


TEST(BreakerTest, SPLITINTO2BLOCKS) {
    Breaker& breaker = Breaker::get_instance();
    std::string code = "int sum (int a, int b) {\n"
    "    return a+b;\n"
    "}\n"
    "\n"
    "int main(int argc, char** argv) {\n"
    "    return 0;\n"
    "}";
    std::queue<Breaker::Block> blocks = breaker.split_in_blocks(code);

    EXPECT_EQ(blocks.front().line_start, 0);
    EXPECT_EQ(blocks.front().line_end, 2);
    EXPECT_EQ(blocks.front().body, "int sum (int a, int b) {\n    return a+b;\n}");


    EXPECT_EQ(blocks.back().line_start, 4);
    EXPECT_EQ(blocks.back().line_end, 6);
    EXPECT_EQ(blocks.back().body, "int main(int argc, char** argv) {\n    return 0;\n}");

}

TEST(BreakerTest, SPLITINTO2BLOCKSBUTMODIFYING) {
    Breaker& breaker = Breaker::get_instance();
    std::string code = 
                        "int sum (int a, int b) {\n"
                        "    return a+b;\n"
                        "}\n"
                        "\n"
                        "int main(int argc, char** argv)\n"
                        "{\n"
                        "    return 0;\n"
                        "}\n";
    std::queue<Breaker::Block> blocks = breaker.split_in_blocks(code);

    EXPECT_EQ(blocks.front().line_start, 0);
    EXPECT_EQ(blocks.front().line_end, 2);
    EXPECT_EQ(blocks.front().body, "int sum (int a, int b) {\n    return a+b;\n}");
    


    EXPECT_EQ(blocks.back().line_start, 4);
    EXPECT_EQ(blocks.back().line_end, 6);
    EXPECT_EQ(blocks.back().body, "int main(int argc, char** argv) {\n    return 0;\n}");
}

TEST(BreakerTest, SPLITINTO2BLOCKSBUTMODIFYINGMORE) {
    Breaker& breaker = Breaker::get_instance();
    std::string code = 
                        "int sum (int a,"
                        "             int b)"
                        "{\n"
                        "    return a+b;\n"
                        "}\n"
                        "\n"
                        "int main(int argc, char** argv)\n"
                        "{\n"
                        "    return 0;\n"
                        "}\n";
    std::queue<Breaker::Block> blocks = breaker.split_in_blocks(code);
//  DEBUG("BODY: " << blocks.front().body)
//  DEBUG("BODY: " << blocks.back().body)

    EXPECT_EQ(blocks.front().line_start, 0);
    EXPECT_EQ(blocks.front().line_end, 2);
    EXPECT_EQ(blocks.front().body, "int sum (int a, int b){\n    return a+b;\n}");
    

    EXPECT_EQ(blocks.back().line_start, 4);
    EXPECT_EQ(blocks.back().line_end, 6);
    EXPECT_EQ(blocks.back().body, "int main(int argc, char** argv) {\n    return 0;\n}");
}


TEST(BreakerTest, SPLITINTO2BLOCKSBUTMODIFYINGMOREANDMORE) {
    Breaker& breaker = Breaker::get_instance();
    std::string code = 
                        "int sum (int a,"
                        "             int b)"
                        "{\n"
                        "    return a+b;\n"
                        "}\n"
                        "\n"
                        "int main(int argc, char** argv)\n"
                        "{\n"
                        "    return 0;\n"
                        "}\n";
    std::queue<Breaker::Block> blocks = breaker.split_in_blocks(code);
//  DEBUG("BODY: " << blocks.front().body)
//  DEBUG("BODY: " << blocks.back().body)

    EXPECT_EQ(blocks.front().line_start, 0);
    EXPECT_EQ(blocks.front().line_end, 2);
    EXPECT_EQ(blocks.front().body, "int sum (int a, int b){\n    return a+b;\n}");
    

    EXPECT_EQ(blocks.back().line_start, 4);
    EXPECT_EQ(blocks.back().line_end, 6);
    EXPECT_EQ(blocks.back().body, "int main(int argc, char** argv) {\n    return 0;\n}");
}

TEST(BreakerTest, SPLITINTO3BLOCKS) {
    Breaker& breaker = Breaker::get_instance();
    std::string code = 
                        "int sum (int a, int b) {\n"
                        "    return a+b;\n"
                        "}\n"
                        "\n"
                        "int main(int argc, char** argv) {\n"
                        "    return 0;\n"
                        "}\n"
                        "int sum2 (int a, int b) {\n"
                        "    return a+b;\n"
                        "}";
    std::queue<Breaker::Block> blocks = breaker.split_in_blocks(code);

    
    (blocks.front().line_start, 0);
    EXPECT_EQ(blocks.front().line_end, 2);
    EXPECT_EQ(blocks.front().body, "int sum (int a, int b) {\n    return a+b;\n}"); 

    blocks.pop();
    EXPECT_EQ(blocks.front().line_start, 4);
    EXPECT_EQ(blocks.front().line_end, 6);
    EXPECT_EQ(blocks.front().body, "int main(int argc, char** argv) {\n    return 0;\n}");

    blocks.pop();
    EXPECT_EQ(blocks.front().line_start, 7);
    EXPECT_EQ(blocks.front().line_end, 9);
    EXPECT_EQ(blocks.front().body, "int sum2 (int a, int b) {\n    return a+b;\n}");
}

TEST(BreakerTest, SPLITINTO3LONGERBLOCKSANDMODIFYING) {
    Breaker& breaker = Breaker::get_instance();
    

    std::string code = 
                        "int function1() {\n"
                            "    int a = 5;\n"
                            "    int b = 10;\n"
                            "    int sum = a + b;\n"
                            "    return sum;\n"
                            "}\n"
                            "\n"
                            "void function2() {\n"
                            "    std::cout << \"Hello, World!\" << std::endl;\n"
                            "}\n"
                            "\n"
                            "float function3(float x, float y) {\n"
                            "    float result = x * y;\n"
                            "    return result;\n"
                            "}";
    std::queue<Breaker::Block> blocks = breaker.split_in_blocks(code);

    EXPECT_EQ(blocks.front().line_start, 0);
    EXPECT_EQ(blocks.front().line_end, 5);
    EXPECT_EQ(blocks.front().body, "int function1() {\n    int a = 5;\n    int b = 10;\n    int sum = a + b;\n    return sum;\n}");

    blocks.pop();
    EXPECT_EQ(blocks.front().line_start, 7);
    EXPECT_EQ(blocks.front().line_end, 9);
    EXPECT_EQ(blocks.front().body, "void function2() {\n    std::cout << \"Hello, World!\" << std::endl;\n}");

    blocks.pop();
    EXPECT_EQ(blocks.front().line_start, 11);
    EXPECT_EQ(blocks.front().line_end, 14);
    EXPECT_EQ(blocks.front().body, "float function3(float x, float y) {\n    float result = x * y;\n    return result;\n}");
}   


TEST(BreakerTest, READACCESS) {
    Breaker& breaker = Breaker::get_instance();
    std::optional<Breaker::Access> access = breaker.extract_access("public:");
    EXPECT_EQ(access.value(), Breaker::Access::PUBLIC);
    access = breaker.extract_access("protected:");
    EXPECT_EQ(access.value(), Breaker::Access::PROTECTED);
    access = breaker.extract_access("private:");
    EXPECT_EQ(access.value(), Breaker::Access::PRIVATE);
    access = breaker.extract_access("default:");
    EXPECT_FALSE(access.has_value());
}

TEST(BreakerTest, READACCESSWITHTEXT) {
    Breaker& breaker = Breaker::get_instance();
    std::optional<Breaker::Access> access = breaker.extract_access("int main(int argc, char** argv) {\n    return 0;\n}\npublic:");
    EXPECT_EQ(access.value(), Breaker::Access::PUBLIC);
    access = breaker.extract_access("int main(int argc, char** argv) {\n    return 0;\n}\nprotected:");
    EXPECT_EQ(access.value(), Breaker::Access::PROTECTED);
    access = breaker.extract_access("int main(int argc, char** argv) {\n    return 0;\n}\nprivate:");
    EXPECT_EQ(access.value(), Breaker::Access::PRIVATE);
    access = breaker.extract_access("int main(int argc, char** argv) {\n    return 0;\n}\ndefault:");
    EXPECT_FALSE(access.has_value());
}