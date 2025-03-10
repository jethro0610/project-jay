#pragma once
#include "Logging/Logger.h"
#include "Types/Alignment.h"
#include "Glyph.h"
#include <array>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

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
