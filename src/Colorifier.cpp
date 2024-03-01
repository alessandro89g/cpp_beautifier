#include "../include/Colorifier.hpp"

const std::unordered_map<Colorifier::Color, std::string> Colorifier::color_map = {
    {Colorifier::Color::RED, "\033[31m"},
    {Colorifier::Color::GREEN, "\033[32m"},
    {Colorifier::Color::YELLOW, "\033[33m"},
    {Colorifier::Color::BLUE, "\033[34m"},
    {Colorifier::Color::MAGENTA, "\033[35m"},
    {Colorifier::Color::CYAN, "\033[36m"},
    {Colorifier::Color::WHITE, "\033[37m"},
    {Colorifier::Color::RESET, "\033[0m"}
};

std::string Colorifier::colorify(const std::string& text, Colorifier::Color color) {
    if (color == Colorifier::Color::RESET) {
        return decolorify(text);
    }
    return color_map.at(color) + text + color_map.at(Colorifier::Color::RESET);
}

std::string Colorifier::decolorify(const std::string& text) {
    std::string result = text;
    for (size_t i = 0; i < result.size(); ++i) {
        if (result[i] == '\033') {
            while (result[i] != 'm') {
                ++i;
            }
            result[i-1] = '0';
            break;
        }
    }
    return result;
}