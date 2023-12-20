#include "StringConversion.h"

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
