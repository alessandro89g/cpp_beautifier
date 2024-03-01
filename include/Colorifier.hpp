#ifndef COLORIFIER_HPP
#define COLORIFIER_HPP
#include <string>
#include <unordered_map>

class Colorifier {
public:
    enum class Color {
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE,
        RESET
    };
    static std::string colorify(const std::string& text, Color color);
    static std::string decolorify(const std::string& text);

private:
    Colorifier();
    Colorifier(const Colorifier&);
    Colorifier(Colorifier&&);
    Colorifier& operator=(const Colorifier&);
    static const std::unordered_map<Color, std::string> color_map;
};


#endif // COLORIFIER_HPP