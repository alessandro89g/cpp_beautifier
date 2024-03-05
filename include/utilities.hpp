#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>
#include <iostream>

std::string remove_leading_trailing_spaces(const std::string& string) {
    std::string str = string;
    while (str[0] == ' ' || str[0] == '\t') {
        str.erase(str.begin());
    }
    while (str[str.size() - 1] == ' ' || str[str.size() - 1] == '\t') {
        str.erase(str.end() - 1);
    }
    return str;
}

size_t parentheses_balance(const std::string& str) {
    size_t balance = 0;
    for (const char& c : str) {
        if (c == '(') {
            balance++;
        } else if (c == ')') {
            balance--;
        }
    }
    return balance;
}

std::vector<std::string> split_string(const std::string& str, const std::string& delimiter = "\n") {
    std::vector<std::string> result;
    size_t pos = 0;
    size_t last_pos = 0;
    while ((pos = str.find(delimiter, last_pos)) != std::string::npos) {
        std::string token = str.substr(last_pos, pos - last_pos);
        if (remove_leading_trailing_spaces(token) != "") {
            result.push_back(token);
        }
        last_pos = pos + delimiter.size();
    }
    std::string token = str.substr(last_pos, str.size() - last_pos);
    if (remove_leading_trailing_spaces(token) != "") {
        result.push_back(token);
    }
    return result;
}









#endif // UTILITIES_H