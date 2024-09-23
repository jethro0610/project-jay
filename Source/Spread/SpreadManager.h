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

class SpreadManager {
public:
    struct RenderData {
        glm::mat4 modelMatrix;
        glm::vec3 color;
        float time;
    };

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

    bool AddSpread(const glm::vec3& position, bool weed = false); 
    bool AddSpread(const SpreadKey& key, bool weed); 
    AddSpreadInfo AddSpread(const glm::vec3& position, int radius, int amount = INT_MAX, bool weed = false);

    bool RemoveSpread(
        const SpreadKey& key,
        Entity* remover = nullptr
    );
    bool RemoveSpread(
        const glm::vec3& position, 
        Entity* remover = nullptr 
    );
    int RemoveSpread(
        const glm::vec3& position, 
        int radius, 
        Entity* remover = nullptr
    ); 
    int RemoveSpread(
        const glm::vec3& position, 
        float radius, 
        Entity* remover = nullptr
    ); 
    void UpdateRenderData_P();

    void* GetSpreadData() { return spreadData_.data(); }
    void* GetWeedData() { return weedData_.data(); }
    SpreadKey GetKey(const glm::vec2& position) const;
    SpreadKey GetKey(const glm::vec3& position) const;

    void Reset();
    int GetSpreadCount() const { return spreadData_.size(); }
    int GetWeedCount() const { return weedData_.size(); }
    int GetCount() const { return spreadData_.size(); }
    float GetCoverage();

    void GetWeedLocations(std::vector<glm::ivec2>& locations);

private:
    struct KeyIndex {
        int index;
        bool weed;
    };

    Terrain& terrain_;
    SeedManager& seedManager_;

    vector_contig<RenderData, MAX_SPREAD> spreadData_;
    vector_contig<RenderData, MAX_SPREAD> weedData_;
    KeyIndex keys_[KEY_LENGTH][KEY_LENGTH];
};
