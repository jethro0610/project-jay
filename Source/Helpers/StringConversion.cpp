#include "StringConversion.h"
#include <sstream>
#include <vector>
#include <vector_const.h>

StringToFloat ToFloat(const std::string& str) {
    try {
        return {
            true,
            std::stof(str)
        };
    }
    catch(...) {
        return {
            false,
            0.0f
        };
    }
}

StringToInt ToInt(const std::string& str) {
    try {
        return {
            true,
            std::stoi(str)
        };
    }
    catch(...) {
        return {
            false,
            0
        };
    }
}

StringToVec2 ToVec2(const std::string& str, const glm::vec2& defaultVec) {
    vector_const<std::string, 2> tokens;
    std::stringstream sstream(str);
    std::string token;

    while(getline(sstream, token, ',')) {
        if (tokens.size() == 2) break;
        tokens.push_back(token);
    }
    if (tokens.size() != 2) return {
        false,
        glm::vec2(0.0f)
    };

    StringToFloat x = ToFloat(tokens[0]);
    StringToFloat y = ToFloat(tokens[1]);

    glm::vec2 returnVec;  
    if (tokens[0] == "_")
        returnVec.x = defaultVec.x;
    else if (x.valid)
        returnVec.x = x.value;
    else
        return { false, glm::vec2(0.0f) };

    if (tokens[1] == "_")
        returnVec.y = defaultVec.y;
    else if (y.valid)
        returnVec.y = y.value;
    else
        return { false, glm::vec2(0.0f) };

    return { true, returnVec };
}
