#pragma once
#include "Glyph.h"
#include "Types/Alignment.h"
#include <string>
#include <array>
#include <glm/vec3.hpp>


class WorldText {
public:
    static constexpr int MAX_CHARS = 64;
    WorldText();
    WorldText(
        const std::string& str, 
        const glm::vec3& position, 
        float size = 1.0f, 
        float hAlignment = Alignment::CENTER, 
        float vAlignment = Alignment::CENTER
    );
    std::array<Glyph, MAX_CHARS> glyphs_;
    int length_;

    struct RenderProperties {
        glm::vec3 position;
        float size;

        float length;
        float hAlignment;
        float vAlignment;
        float padding;
    };
    RenderProperties properties_;

    void RemoveLast() {
        if (length_ == 0) return;
        length_--;
        properties_.length = length_;
    }

    void SetString(const std::string& str) {
        for (int i = 0; i < MAX_CHARS; i++) {
            if (i < str.length())
                glyphs_[i].character = (float)str[i] - 32;
            else
                glyphs_[i].character = (float)' ' - 32;
        }
        length_ = str.length();
        properties_.length = length_;
    }

    void Clear() {
        length_ = 0;
        properties_.length = 0;
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
        properties_.length = length_;
    }

    void operator=(const std::string& str) { 
        SetString(str);
    }
};
