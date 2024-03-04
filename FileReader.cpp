#include "FileReader.h"

#define DEBUG(...)  cout << __VA_ARGS__ << endl;

#define _S "\\s+"                                           //  One or more of any whitespace character: ' ', '\t', '\v', '\f', '\r'
#define _s "\\s*"                                           // Zero or more of any whitespace character: ' ', '\t', '\v', '\f', '\r'
#define NAME_BASE "[a-zA-Z_][a-zA-Z0-9_]*"                       // Examples: "std" "filesystem" "Beautifier" "AccessSpecifier"
#define NAMESPACE "(?:" NAME_BASE _s "::" _s ")*"                // Examples: "std::" "std::filesystem::"
#define TYPE_BASE "(?:" NAMESPACE NAME_BASE _s "(<.+?>)?)"    
#define CONST "(?:const" _S ")?"
#define TYPE "(" CONST TYPE_BASE "(?:\\*|&)?)" _s  // Examples: "const std::string" "const std::filesystem::path" "const Beautifier*" "const vector<Beautifier>&"
#define ARG "(((?:" TYPE _S NAME_BASE ")))"
#define ARGS "\\((" _s ARG ",?)*\\)"         //"\\((" _s ARG ",?)*\\)"                  // Examples: "(const std::string name, const std::filesystem::path header, const vector<Beautifier>& classes)"
#define PRE_MODIFIERS "(static|virtual|abstract|inline|mutable|explicit|extern|noexcept|constexpr)"
#define POST_MODIFIERS  "(?:\\s+(?:const|throws|volatile|noexcept|throws))*"
#define METHOD_BASE  "\\b(?!" PRE_MODIFIERS "\\b)" TYPE "\\b(" NAME_BASE ")" _s ARGS             // Examples: "const std::string getHeader()" "const std::filesystem::path getSource()" "const vector<Beautifier>& getClasses()"

#define METHOD "(" PRE_MODIFIERS _s ")?" METHOD_BASE  POST_MODIFIERS "[;|{]" // Examples: "const std::string getHeader() const" "const std::filesystem::path getSource() const" "const vector<Beautifier>& getClasses() const"

#define MEMBER TYPE _S "(" NAME_BASE "))\\h*[,|\\)|;])"


int main(int argc, char const *argv[]) {
    FileReader file_reader(argv[1]);
    
    string text = file_reader.file_content;



    DEBUG("")
    DEBUG("")
    DEBUG("")
    DEBUG("")
    smatch match;
    const string pattern_string = METHOD;
    regex pattern(pattern_string);

    uint count = 0;
    const string file_content = text;
    while(regex_search(text,match,pattern)) {
        count++;
        DEBUG(count << "    " << match.str())
        text = match.suffix();
    }
    DEBUG(pattern_string)
    return 0;

    // DEBUG("")
    // DEBUG("")
    // DEBUG("")
    // DEBUG("")


    // const string test = file_content;
    // string to_check = METHOD;
    // pattern.assign(to_check);
    // string::const_iterator search_start(test.cbegin());
    // count = 0;
    // while (regex_search(search_start, test.cend(), match, pattern)) {
    //     count++;
    //     DEBUG(count << "  ->  " << match.str())
    //     search_start = match.suffix().first;
    // }
    // DEBUG(to_check)


    return 0;
}