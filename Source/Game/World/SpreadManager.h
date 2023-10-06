#pragma once
#include <set>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <deque>
#include <unordered_set>
#include <algorithm>
#include <vector_contig.h>
#include "../../Constants/SpreadConstants.h"
#include "../Entity/EntityID.h"
#include "SpreadKey.h"
class World;
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

class SpreadManager {
public:
    SpreadManager(
        SeedManager& seedManager,
        World& world
    );
    SpreadManager(const SpreadManager&) = delete;

    bool SpreadIsActive(const glm::vec2& position) const;
    bool SpreadIsActive(const glm::vec3& position) const;

    bool AddSpread(const glm::vec3& position); 
    AddSpreadInfo AddSpread(const glm::vec3& position, int radius, int amount = INT_MAX);

    bool RemoveSpread(
        const glm::vec3& position, 
        EntityID remover = NULL_ENTITY, 
        const glm::vec3& seedOffset = glm::vec3(0.0f)
    );
    int RemoveSpread(
        const glm::vec3& position, 
        int radius, 
        EntityID remover = NULL_ENTITY, 
        const glm::vec3& seedOffset = glm::vec3(0.0f)
    ); 
    void UpdateRenderData_P();

    int GetCount() const { return count_; }
    SpreadRenderData* GetRenderData() { return renderData_.data(); }
    SpreadKey GetKey(const glm::vec2& position) const;
    SpreadKey GetKey(const glm::vec3& position) const;

    void ClearTramples() { tramples_.clear(); }
    void Trample(const SpreadKey& key);
    void Trample(const glm::vec3& position);
    void Trample(const glm::vec3& position, int radius);

private:
    bool AddSpread(const SpreadKey& key); 
    bool RemoveSpread(
        const SpreadKey& key,
        EntityID remover = NULL_ENTITY,
        const glm::vec3& seedOffset = glm::vec3(0.0f)
    );

    World& world_;
    SeedManager& seedManager_;
    vector_contig<SpreadKey, 8192> viableAddKeys_;

    vector_contig<SpreadRenderData, MAX_SPREAD> renderData_;
    std::unordered_map<SpreadKey, int> keyIndices_;
    std::unordered_set<SpreadKey> tramples_;
    int count_;
};
