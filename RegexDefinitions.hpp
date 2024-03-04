#ifndef REGEX_DEFINITIONS_HPP
#define REGEX_DEFINITIONS_HPP

// Basic definitions to be combined
#define _S "\\s+"                                           //  One or more of any whitespace character: ' ', '\t', '\v', '\f', '\r'
#define _s "\\s*"                                           // Zero or more of any whitespace character: ' ', '\t', '\v', '\f', '\r'
#define NAME_BASE "[a-zA-Z_][a-zA-Z0-9_]*"
#define NAMESPACE "(?:" NAME_BASE _s "::" _s ")*"
#define TYPE_BASE "(?:" NAMESPACE NAME_BASE _s "(<.+?>)?)"
#define CONST "(?:const" _S ")?"
#define TYPE "(" CONST TYPE_BASE "(?:\\*|&|&&)?)" _s
#define ARG "(((?:" TYPE _S NAME_BASE ")))"
#define ARGS "\\((" _s ARG ",?)*\\)"
#define METHOD_BASE  "\\b(?!" PRE_MODIFIERS "\\b)" TYPE "\\b(" NAME_BASE ")" _s ARGS


// Keywords to be combined in the regex
#define PRE_MODIFIERS "(static|virtual|abstract|inline|mutable|explicit|extern|noexcept|constexpr)"
#define POST_MODIFIERS  "(?:\\s+(?:const|throws|volatile|noexcept|throws))*"
#define ACCESS_SPECIFIER "(public|protected|private)"

// Definitions to be used in the regex
#define METHOD_RGX "(" PRE_MODIFIERS _s ")?" METHOD_BASE  POST_MODIFIERS "[;|\\{]"
#define CLASS_NAME "\\h*(class|struct)\\s+(" NAME_BASE ")(\\s*:\\s+(" ACCESS_SPECIFIER "?\\s*" TYPE_BASE "\\,?\\s?)*\\s*)?\\s*(;|\\{)"

#define MEMBER_RGX TYPE _S "(" NAME_BASE "))\\h*[,|\\)|;])"

#define CONSTRUCTOR(class_name) "(\\h*(" PRE_MODIFIERS _S ")*" class_name _s ARGS _s "(?:" POST_MODIFIERS _s ")?)"

#endif // REGEX_DEFINITIONS_HPP