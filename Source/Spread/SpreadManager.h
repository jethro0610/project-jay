#pragma once
#include "SpreadKey.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_set>
#include <vector_contig.h>

class Entity;
class Terrain;
class SeedManager;
class TransformComponent;

struct AddSpreadInfo {
    int count;
    SpreadKey key;
};

struct SpreadRenderData {
    glm::mat4 modelMatrix;
    glm::vec3 color;
    float time;
};

struct SpreadParticle {
    glm::vec4 position;
    float time;
    float padding[11];
};

class SpreadManager {
public:
    static constexpr float SPREAD_DIST = 2.0f;
    static constexpr int MAX_SPREAD = 32768;
    static constexpr int KEY_LENGTH = 1024;
    static constexpr int MAX_SPREAD_PARTICLES = 4096;

    SpreadManager(
        SeedManager& seedManager,
        Terrain& terrain
    );
    SpreadManager(const SpreadManager&) = delete;

    bool SpreadIsActive(const glm::vec2& position) const;
    bool SpreadIsActive(const glm::vec3& position) const;

    bool AddSpread(const glm::vec3& position); 
    AddSpreadInfo AddSpread(const glm::vec3& position, int radius, int amount = INT_MAX);

    bool RemoveSpread(
        const glm::vec3& position, 
        bool createSeed = false,
        Entity* remover = nullptr, 
        const glm::vec3& seedOffset = glm::vec3(0.0f)
    );
    int RemoveSpread(
        const glm::vec3& position, 
        int radius, 
        bool createSeed = false,
        Entity* remover = nullptr, 
        const glm::vec3& seedOffset = glm::vec3(0.0f)
    ); 
    void UpdateRenderData_P();

    int GetCount() const { return count_; }
    SpreadRenderData* GetRenderData() { return renderData_.data(); }
    SpreadKey GetKey(const glm::vec2& position) const;
    SpreadKey GetKey(const glm::vec3& position) const;

    float particleTimer_;
    vector_contig<SpreadParticle, MAX_SPREAD_PARTICLES> spreadParticles_;
    void Update(float deltaTime);
    void Reset();
    float GetCoverage();

private:
    bool AddSpread(const SpreadKey& key); 
    bool RemoveSpread(
        const SpreadKey& key,
        bool createSeed = false,
        Entity* remover = nullptr,
        const glm::vec3& seedOffset = glm::vec3(0.0f)
    );

    Terrain& terrain_;
    SeedManager& seedManager_;

    vector_contig<SpreadRenderData, MAX_SPREAD> renderData_;
    int spreadKeys_[KEY_LENGTH][KEY_LENGTH];
    int count_;
};
