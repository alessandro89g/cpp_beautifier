#include "../include/Breaker.hpp"
#include "../include/utilities.hpp"

using namespace std;

Breaker& Breaker::get_instance() {
    static Breaker instance;
    return instance;
}

Breaker::Method Breaker::read_method(const Block& block, Access access) const {
    return read_method(static_cast<const char*>(block), block.line_start, access);
}

Breaker::Method Breaker::read_method(const string& string_method, uint line_start, Access access) const {
//  DEBUG("===============================")
//  DEBUG(string_method)
    Method method;
    method.body.line_start = line_start;
    method.body.line_end = line_start + lines_in_block(string_method);
    method.access = access;
    method.type = Type::METHOD;
    method.body = read_body(string_method);

    if (string_method.find_last_of(';') != string::npos) {
        method.definition_in_header = false;
    }
    if (string_method.find('{') != string::npos) {
        method.definition_in_header = true;
    }

    string head = string_method.substr(0, string_method.find("(") );
    string post_modifiers = string_method.substr(string_method.find(")") + 1);
    if (post_modifiers != ";") {
        clear_string(post_modifiers);
        if (post_modifiers.find(";") != string::npos) {
            post_modifiers = post_modifiers.substr(0, post_modifiers.find(";"));
        }
        if (post_modifiers != "") {
            method.post_modifiers = read_modifiers(post_modifiers);
        }
    }
//  DEBUG("POST MODIFIERS: " << post_modifiers)
    while (head.find_last_of(" ") == head.length() - 1) {
        head = head.substr(0, head.length() - 1);
    }
    method.name = head.substr(head.find_last_of(" ") + 1);
    head = head.substr(0, head.find_last_of(" "));
    method.return_type = head.substr(head.find_last_of(" ") + 1);
    if (head.find_last_of(" ") != string::npos)
        head = head.substr(0, head.find_last_of(" "));
    else
        head = "";
    method.pre_modifiers = read_modifiers(head);
    method.args = read_args(string_method.substr(string_method.find("("), string_method.find(")") - string_method.find("(") + 2));

//  DEBUG(method_to_string(method, true))

    return method;
}

vector<Breaker::Modifier> Breaker::read_modifiers(const string& string_modifiers) const {
    vector<Modifier> modifiers;
    vector<string> tokens = string_split(string_modifiers, " ");
    for (string& token : tokens) {
        clear_string(token);
        if (token == "const") {
            modifiers.push_back(Modifier::CONST);
        } else if (token == "static") {
            modifiers.push_back(Modifier::STATIC);
        } else if (token == "virtual") {
            modifiers.push_back(Modifier::VIRTUAL);
        } else if (token == "abstract") {
            modifiers.push_back(Modifier::ABSTRACT);
        } else if (token == "inline") {
            modifiers.push_back(Modifier::INLINE);
        } else if (token == "mutable") {
            modifiers.push_back(Modifier::MUTABLE);
        } else if (token == "explicit") {
            modifiers.push_back(Modifier::EXPLICIT);
        } else if (token == "extern") {
            modifiers.push_back(Modifier::EXTERN);
        } else if (token == "noexcept") {
            modifiers.push_back(Modifier::NOEXCEPT);
        } else if (token == "constexpr") {
            modifiers.push_back(Modifier::CONSTEXPR);
        }
    }
    return modifiers;
}

void Breaker::clear_string(string& str) const {
    if (str == "" || str == " ") {
        return;
    }
    while (str.find_first_of(" ") == 0) {
        str = str.substr(1);
    }
    while (str.find_last_of(" ") == str.length() - 1) {
        str = str.substr(0, str.length() - 1);
    }
}

uint Breaker::lines_in_block(const string& block) const {
    uint lines = 0;
    for (const char& c : block) {
        if (c == '\n') {
            lines++;
        }
    }
    return lines;
}


vector<string> Breaker::read_args(string string_args) const {
    string_args = string_args.substr(1, string_args.find(")")-1);
    vector<string> args = string_split(string_args, ",");
    for (string& arg : args) {
        clear_string(arg);
    }
    args.erase(remove_if(args.begin(), args.end(), [](const string& str) { return str == ""; }), args.end());
    return args;
}

string Breaker::read_body(const string& string_method) const {
    if (string_method.find("{") == string::npos) {
        return "";
    }
    string body = string_method.substr(string_method.find("{") + 1, string_method.find_last_of("}") - string_method.find("{") - 1);
    // body = remove_leading_trailing_spaces(body);
    body = remove_leading_trailing_new_lines(body);
    return body;
}

string Breaker::method_to_string(const Method& method, bool more_info = false) const {
    stringstream ss;
    for (const Modifier& modifier : method.pre_modifiers) {
        ss << modifier << " ";
    }
    ss << method.return_type << " " << method.name;
    if (method.args.size() == 0) {
        ss << "()";
    }
    else {
        ss << "( ";
        for (size_t i = 0; i < method.args.size(); i++) {
            ss << method.args[i];
            if (i != method.args.size() - 1)
                ss << ", ";
        }
        ss << " )";
    }
    for (const Modifier& modifier : method.post_modifiers) {
        ss << " " << modifier;
    }
    if (more_info) {
        if (method.body.body != "") 
            ss << " {\n " << static_cast<const char*>(method.body) << "\n}\n\n";
        else 
            ss << ";\n\n";
        ss << " Access: " << method.access << '\n';
        ss << " Line start: " << method.body.line_start << "\n";
        ss << " Line end: " << method.body.line_end << "\n";
    }

    return ss.str();
}

ostream& operator<<(ostream& os, const Breaker::Access& access) {
    switch (access) {
        case Breaker::Access::PUBLIC:
            os << "public";
            break;
        case Breaker::Access::PROTECTED:
            os << "protected";
            break;
        case Breaker::Access::PRIVATE:
            os << "private";
            break;
        default:
            throw std::runtime_error("Tring to print unknown access");
    }
    return os;
}

ostream& operator<<(ostream& os, const Breaker::Modifier& modifier) {
    switch (modifier) {
        case Breaker::Modifier::CONST:
            os << "const";
            break;
        case Breaker::Modifier::STATIC:
            os << "static";
            break;
        case Breaker::Modifier::VIRTUAL:
            os << "virtual";
            break;
        case Breaker::Modifier::ABSTRACT:
            os << "abstract";
            break;
        case Breaker::Modifier::INLINE:
            os << "inline";
            break;
        case Breaker::Modifier::MUTABLE:
            os << "mutable";
            break;
        case Breaker::Modifier::EXPLICIT:
            os << "explicit";
            break;
        case Breaker::Modifier::EXTERN:
            os << "extern";
            break;
        case Breaker::Modifier::NOEXCEPT:
            os << "noexcept";
            break;
        case Breaker::Modifier::CONSTEXPR:
            os << "constexpr";
            break;
        default:
            throw std::runtime_error("Tring to print unknown modifier");
    }
    return os;
}

std::queue<Breaker::Block> Breaker::split_in_blocks(const std::string& str) const {
    std::string text = remove_leading_trailing_spaces(str);
    // Removing curly braces at beginning of lines
    while (regex_search(text, std::regex("\\n\\{"))) {
        text = regex_replace(text, std::regex("\\n\\{"), " {");
    }
    // Writing the class declaration in one line
    while (regex_search(text, std::regex("(\\h*)\\bclass\\b(.+)\\n(.+)\\{"))) {
        text = regex_replace(text, std::regex("(\\h*)\\bclass\\b(.+)\\n(.+)\\{"), "$1class$2 $3 {");
    }

    // Writing the constructor initialization list in one line
    while (regex_search(text, std::regex("(\\s*)\\n(\\s*)\\:((.+?)\\{)"))) {
        text = regex_replace(text, std::regex("(\\s*)\\n(\\s*)\\:\\s*((.+?)\\{)"), " : $3");
    }
    // DEBUG("===================================================")
    // DEBUG(text);
    // DEBUG("(\\s*)\\n(\\s*)\\:\\s*((.+?)\\{)")
    // DEBUG("===================================================")

    std::queue<Block> blocks;
    std::vector<std::string> lines = string_split(text, "\n");
    size_t p_balance;
    size_t line_count;
    
    for (size_t i = 0; i < lines.size(); i++) {
        if (lines[i] == "" || lines[i] == "\n") {
            continue;
        }
        Block block(remove_trailing_spaces(lines[i]), i);
        line_count = 0;
        p_balance = parentheses_balance(lines[i], '{');
        if (p_balance != 0) {
            while(p_balance != 0) {
                i++;
                string to_add = remove_trailing_spaces(lines[i]);
                if (to_add[0]=='{')
                    to_add = ' ' + to_add;
                block += '\n' + to_add;
                line_count++;
                p_balance += parentheses_balance(lines[i], '{');
            }
        }
        block = remove_multiple_spaces_between_words(block.body);
        block = remove_trailing_new_lines(block.body);
        blocks.emplace(std::move(block));
    }
    return blocks;
}