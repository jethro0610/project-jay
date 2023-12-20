#include <string>

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
