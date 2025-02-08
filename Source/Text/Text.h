#pragma once
#include "Logging/Logger.h"
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

    float hAlignment;
    float vAlignment;
    float hAnchor;
    float vAnchor;

    float count;
    glm::vec3 padding;
};

class Text {
public:
    static constexpr int MAX_CHARS = 64;
    static constexpr float LEFT_ALIGN = 0.0f;
    static constexpr float RIGHT_ALIGN = 1.0f;
    static constexpr float TOP_ALIGN = 0.0f;
    static constexpr float BOTTOM_ALIGN = 1.0f;
    static constexpr float CENTER_ALIGN = 0.5f;

    Text(const glm::vec2& position, float scale, float kerning);
    Text();
    TextProperties properties_;
    std::array<Glyph, MAX_CHARS> glyphs_;
    int length_;

    void RemoveLast() {
        if (length_ == 0) return;
        length_--;
    }

    void Clear() {
        length_ = 0;
    }

    std::string ToString() {
        std::string str;
        for (int i = 0; i < length_; i++)
            str += (char)glyphs_[i].character + 32; 
        return str;
    }

    void operator+=(const std::string& str) {
        assert(length_ + str.length() <= MAX_CHARS);
        for (int i = 0; i < str.length(); i++) {
            glyphs_[i + length_].character = str[i];
        }
        length_ += str.length();
    }

    void operator+=(int chr) {
        assert(length_ + 1 <= MAX_CHARS);
        glyphs_[length_].character = chr - 32;
        length_++;
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
