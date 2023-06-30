#pragma once
#include <set>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <deque>
#include <unordered_set>
#include <algorithm>
#include "../../Constants/WorldConstants.h"
#include "../../Constants/SpreadConstants.h"
#include "../../Types/FixedVector.h"
#include "../../Types/EntityTypes.h"
class World;
class SeedManager;
class TransformComponent;

struct AddSpreadInfo {
    uint16_t count;
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
    AddSpreadInfo AddSpread(glm::vec3 position, int radius, uint16_t amount = UINT16_MAX);

    bool RemoveSpread(
        glm::vec3 position, 
        EntityIDNullable remover = NO_ENTITY, 
        glm::vec3 seedOffset = glm::vec3(0.0f)
    );
    uint16_t RemoveSpread(
        glm::vec3 position, 
        int16_t radius, 
        EntityIDNullable remover = NO_ENTITY, 
        glm::vec3 seedOffset = glm::vec3(0.0f)
    ); 
    void UpdateRenderData_P();

    uint32_t GetCount() const { return count_; }
    glm::vec4* GetPositions() const { return positions_.GetData(); }
    SpreadKey GetKey(glm::vec2 position) const;
    SpreadKey GetKey(glm::vec3 position) const;

private:
    bool AddSpread(SpreadKey key); 
    bool RemoveSpread(
        SpreadKey key,
        EntityIDNullable remover = NO_ENTITY,
        glm::vec3 seedOffset = glm::vec3(0.0f)
    );

    World& world_;
    SeedManager& seedManager_;
    FixedVector<SpreadKey, 8192> viableAddKeys_;

    FixedVector<glm::vec4, MAX_SPREAD> positions_;
    std::unordered_map<SpreadKey, uint32_t> keyIndices_;
    uint32_t count_;
};
