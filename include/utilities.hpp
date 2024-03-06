#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>
#include <iostream>

#ifndef DEBUG
    #define DEBUG(...) std::cout << __VA_ARGS__ << std::endl;
#endif // DEBUG

std::string remove_trailing_new_lines(const std::string& string);

std::string remove_trailing_spaces(const std::string& string);

std::string remove_leading_spaces(const std::string& string);

std::string remove_leading_trailing_spaces(const std::string& string);

// create a function that eliminates a string from inside the string, for example
// eliminate_substrings_in_strings("return \"Ciao \\\"amore\\\\"\";") -> "return ;"
std::string eliminate_substrings_in_strings(const std::string& str); 

std::string eliminate_parentheses_in_substrings_and_chars(const std::string& string, char open_parenthesis_type = '(');

size_t parentheses_balance(const std::string& string, char open_parenthesis_type = '('); 

std::vector<std::string> string_split(const std::string& str, const std::string& delimiter = "\n");

std::string string_join(const std::vector<std::string>& strings, const std::string& delimiter = "\n");

std::vector<std::string> split_in_blocks(const std::string& str);

#endif // UTILITIES_H