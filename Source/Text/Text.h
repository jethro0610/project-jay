#pragma once
#include <array>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Glyph {
    float character;
    glm::vec3 padding;
};

struct TextProperties {
    glm::vec2 position;
    float scale;
    float kerning;
};

class Text {
public:
    static constexpr int MAX_CHARS = 64;
    Text(const glm::vec2& position, float scale, float kerning);
    Text();
    TextProperties properties_;
    std::array<Glyph, MAX_CHARS> glyphs_;

    float& operator[](int index) {
        return glyphs_[index].character;
    }

    void operator=(const std::string& text) { 
        for (int i = 0; i < MAX_CHARS; i++) {
            if (i < text.length())
                glyphs_[i].character = (float)text[i] - 32;
            else
                glyphs_[i].character = (float)' ' - 32;
        }
    }
};
