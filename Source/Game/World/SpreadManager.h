#pragma once
#include <set>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <deque>
#include <unordered_set>
#include <algorithm>
#include "../../Constants/SpreadConstants.h"
#include "../../Types/FixedVector.h"
#include "../Entity/EntityID.h"
#include "SpreadKey.h"
class World;
class SeedManager;
class TransformComponent;


struct AddSpreadInfo {
    int count;
    SpreadKey key;
};

class SpreadManager {
public:
    SpreadManager(
        SeedManager& seedManager,
        World& world
    );
    SpreadManager(const SpreadManager&) = delete;

    bool SpreadIsActive(glm::vec2 position) const;
    bool SpreadIsActive(glm::vec3 position) const;

    bool AddSpread(glm::vec3 position); 
    AddSpreadInfo AddSpread(glm::vec3 position, int radius, int amount = INT_MAX);

    bool RemoveSpread(
        glm::vec3 position, 
        EntityID remover = NULL_ENTITY, 
        glm::vec3 seedOffset = glm::vec3(0.0f)
    );
    int RemoveSpread(
        glm::vec3 position, 
        int radius, 
        EntityID remover = NULL_ENTITY, 
        glm::vec3 seedOffset = glm::vec3(0.0f)
    ); 
    void UpdateRenderData_P();

    int GetCount() const { return count_; }
    glm::mat4* GetTransforms() const { return transforms_.GetData(); }
    SpreadKey GetKey(glm::vec2 position) const;
    SpreadKey GetKey(glm::vec3 position) const;

private:
    bool AddSpread(SpreadKey key); 
    bool RemoveSpread(
        SpreadKey key,
        EntityID remover = NULL_ENTITY,
        glm::vec3 seedOffset = glm::vec3(0.0f)
    );

    World& world_;
    SeedManager& seedManager_;
    FixedVector<SpreadKey, 8192> viableAddKeys_;

    FixedVector<glm::mat4, MAX_SPREAD> transforms_;
    std::unordered_map<SpreadKey, int> keyIndices_;
    int count_;
};
