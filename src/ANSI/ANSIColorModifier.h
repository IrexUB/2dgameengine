#pragma once

#include <string>
#include <sstream>

constexpr std::string_view g_resetSequence = "\e[0m";

namespace ansi_cm
{
    enum class Color
    {
        BLACK       = 30,
        RED         = 31,
        GREEN       = 32,
        YELLOW      = 33,
        BLUE        = 34,
        PURPLE      = 35,
        CYAN        = 36,
        WHITE       = 37
    };

    enum class FontStyle
    {
        REGULAR     = 0,
        BOLD        = 1,
        UNDERLINE   = 4,
    };

    class ColorModifier
    {
    public:
        using enum Color;
        using enum FontStyle;

        static std::string Modify(std::string_view str, Color color = WHITE, FontStyle style = REGULAR)
        {
            std::stringstream coloredStrStream;
            coloredStrStream << "\e[" << static_cast<int>(style) << ';' << static_cast<int>(color) << 'm' << str << g_resetSequence;

            return coloredStrStream.str();
        }
    };
}