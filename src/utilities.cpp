#include "../include/utilities.hpp"

std::string remove_trailing_new_lines(const std::string& string) {
    std::string str = string;
    while (str[str.size() - 1] == '\n') {
        str.erase(str.end() - 1);
    }
    return str;
}

std::string remove_trailing_spaces(const std::string& string) {
    std::string str = string;
    while (str[str.size() - 1] == ' ' || str[str.size() - 1] == '\t') {
        str.erase(str.end() - 1);
    }
    return str;
}

std::string remove_leading_spaces(const std::string& string) {
    std::string str = string;
    while (str[0] == ' ' || str[0] == '\t') {
        str.erase(str.begin());
    }
    return str;
}

std::string remove_leading_trailing_spaces(const std::string& string) {
    std::string str = remove_leading_spaces(string);
    str = remove_trailing_spaces(str);
    return str;
}


// create a function that eliminates a string from inside the string, for example
// eliminate_substrings_in_strings("return \"Ciao \\\"amore\\\\"\";") -> "return ;"
std::string eliminate_substrings_in_strings(const std::string& str) {
    std::string result;
    bool in_string = false;
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == '"') {
            if (i==0) {
                in_string = !in_string;
                continue;

            } else if (str[i-1]!= '\\') {
                in_string = !in_string;
                continue;
            }
        }
        if (!in_string) {
            result += str[i];
        }
    }
    return result;
}

std::string eliminate_parentheses_in_substrings_and_chars(const std::string& string, char open_parenthesis_type) {
    std::string tmp = eliminate_substrings_in_strings(string);
    std::string result;
    char open = open_parenthesis_type;
    char close;
    switch (open_parenthesis_type)
    {
    case '(':
        close = ')';
        break;
    case '[':
        close = ']';
        break;  
    case '{':
        close = '}';
        break;      
    default:
        throw std::invalid_argument("Invalid open parenthesis type");
    }
    for (size_t i = 0; i < tmp.size(); i++)
    {
        if (tmp[i]=='\'') {
            if (tmp[i+1] == open  || tmp[i+1] == close) {
                i+= 2;
                continue;
            }
        }
        result += tmp[i];
    }
    return result;    
}

size_t parentheses_balance(const std::string& string) {
    std::string str = eliminate_parentheses_in_substrings_and_chars(string);
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

std::vector<std::string> split_string(const std::string& str, const std::string& delimiter) {
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

