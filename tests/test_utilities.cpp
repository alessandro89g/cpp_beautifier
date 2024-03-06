#include <gtest/gtest.h>
#include "../include/utilities.hpp"

TEST(SplitStringTest, EmptyString) {
    std::string str = "";
    std::vector<std::string> result = split_string(str);
    ASSERT_EQ(result.size(), 0);
}

TEST(SplitStringTest, SingleDelimiter) {
    std::string str = "Hello\nWorld";
    std::vector<std::string> result = split_string(str);
    ASSERT_EQ(result.size(), 2);
    ASSERT_EQ(result[0], "Hello");
    ASSERT_EQ(result[1], "World");
}

TEST(SplitStringTest, MultipleDelimiters) {
    std::string str = "Hello\nWorld\nThis\nIs\nA\nTest";
    std::vector<std::string> result = split_string(str, "\n");
    ASSERT_EQ(result.size(), 6);
    ASSERT_EQ(result[0], "Hello");
    ASSERT_EQ(result[1], "World");
    ASSERT_EQ(result[2], "This");
    ASSERT_EQ(result[3], "Is");
    ASSERT_EQ(result[4], "A");
    ASSERT_EQ(result[5], "Test");
}

TEST(RemoveLeadingTrailingSpacesTest, NoSpaces) {
    std::string str = "Hello";
    str = remove_leading_trailing_spaces(str);
    ASSERT_EQ(str, "Hello");
}

TEST(RemoveLeadingTrailingSpacesTest, LeadingSpaces) {
    std::string str = "   Hello";
    str = remove_leading_trailing_spaces(str);
    ASSERT_EQ(str, "Hello");
}

TEST(RemoveLeadingTrailingSpacesTest, TrailingSpaces) {
    std::string str = "Hello   ";
    str = remove_leading_trailing_spaces(str);
    ASSERT_EQ(str, "Hello");
}

TEST(RemoveLeadingTrailingSpacesTest, LeadingAndTrailingSpaces) {
    std::string str = "   Hello   ";
    str = remove_leading_trailing_spaces(str);
    ASSERT_EQ(str, "Hello");
}

TEST(ParenthesesBalanceTest, BalancedString) {
    std::string str = "(a + b) * (c - d)";
    size_t balance = parentheses_balance(str);
    ASSERT_EQ(balance, 0);
}

TEST(ParenthesesBalanceTest, UnbalancedString) {
    std::string str = "(a + b) * (c - d";
    size_t balance = parentheses_balance(str);
    ASSERT_EQ(balance, 1);
}

TEST(ParenthesesBalanceTest, UnbalancedString2) {
    std::string str = "(((a + b * c - d)";
    size_t balance = parentheses_balance(str);
    ASSERT_EQ(balance, 2);
}

TEST(ParenthesesBalanceTest, NoParentheses) {
    std::string str = "a + b * c - d";
    size_t balance = parentheses_balance(str);
    ASSERT_EQ(balance, 0);
}

TEST(EliminateStringsTest, NoStrings) {
    std::string str = "Hello World";
    std::string result = eliminate_substrings_in_strings(str);
    ASSERT_EQ(result, "Hello World");
}

TEST(EliminateStringsTest, SingleString) {
    std::string str = "return \"Ciao \\\"amore\\\"\";";
    std::string result = eliminate_substrings_in_strings(str);
    ASSERT_EQ(result, "return ;");
}

TEST(EliminateStringsTest, MultipleStrings) {
    std::string str = "\"Hello\" World \"This\" is \"a\" Test";
    std::string result = eliminate_substrings_in_strings(str);
    ASSERT_EQ(result, " World  is  Test"); 
}

TEST(EliminateStringsTest, EmptyString) {
    std::string str = "";
    std::string result = eliminate_substrings_in_strings(str);
    ASSERT_EQ(result, "");
}

TEST(EliminateParenthesesInSubstringsAndChars, parenthesesInChar) {
    std::string str = "    char a = '(';";
    std::string result = eliminate_parentheses_in_substrings_and_chars(str);
    ASSERT_EQ(result, "    char a = ;");

}

TEST(EliminateParenthesesInSubstringsAndChars, parenthesesInCharAndString) {
    std::string str = "    return '('~+~\"Ciao\"~+~')';";
    std::string result = eliminate_parentheses_in_substrings_and_chars(str);
    ASSERT_EQ(result, "    return ~+~~+~;");
}