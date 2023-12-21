#include <string>
#include <glm/vec2.hpp>

struct StringToFloat {
    bool valid;
    float value;
};
StringToFloat ToFloat(const std::string& str);

struct StringToInt {
    bool valid;
    int value;
};
StringToInt ToInt(const std::string& str);

struct StringToVec2 {
    bool valid;
    glm::vec2 value;
};
StringToVec2 ToVec2(const std::string& str, const glm::vec2& defaultVec = glm::vec2(0.0f));
