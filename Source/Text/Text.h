#pragma once
#include <array>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Glyph {
    float index;
    float character;
    glm::vec2 padding;
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
    int length_;

    float& operator[](int index) {
        assert(index < length_);
        return glyphs_[index].character;
    }

    void operator=(const std::string& str) { 
        for (int i = 0; i < MAX_CHARS; i++) {
            if (i < str.length())
                glyphs_[i].character = (float)str[i] - 32;
            else
                glyphs_[i].character = (float)' ' - 32;
        }
        length_ = str.length();
    }
};
