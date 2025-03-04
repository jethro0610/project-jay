#pragma once
#include <FastNoiseLite.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class GlobalNoise {
private:
    GlobalNoise();
    static GlobalNoise* instance_;
    FastNoiseLite noise_;

public: 
    static void Init();
    static float Sample(float x, float y);
    static float Sample(const glm::vec2& pos);
    static float Sample(const glm::vec3& pos);
};
