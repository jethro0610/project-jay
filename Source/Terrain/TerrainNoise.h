#pragma once
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <math.h>
#include <FastNoiseLite.h>
#include <glm/gtx/norm.hpp>
#include "StaticTerrainModifier.h"
#include "Editor/EditorTarget.h"

class TerrainNoise : public StaticTerrainModifier {
public:
    class ETarget : public EditorTarget {
    private:
        friend TerrainNoise;
        TerrainNoise* noise_;

    public:
        std::string GetName() override { return "e_noise"; }

        glm::vec3 GetPosition() override;
        void SetPosition(const glm::vec3 &pos) override;

        glm::vec4 GetScale() override;
        void SetScale(const glm::vec4& ref, const glm::vec4 &scale) override;

        bool UpdateTerrain() override { return true; }
        bool RayHit(const Ray &ray, Terrain& terrain) override;
    };

    TerrainNoise();
    TerrainNoise(int seed, glm::vec2 position, float radius, float minHeight, float maxHeight, float frequency);

    FastNoiseLite noise_;
    int seed_;
    glm::vec2 position_;
    float radius_;
    float minHeight_;
    float maxHeight_;
    float frequency_;

    ETarget editorTarget_;

    bool InRange(const glm::vec2& pos) override;
    float GetHeight(const glm::vec2& pos) override;
};
