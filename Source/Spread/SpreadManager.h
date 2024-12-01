#pragma once
#include "SpreadKey.h"
#include "SpreadType.h"
#include "Types/Cone.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
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
    struct KeyIndex {
        int index;
        SpreadType type;
    };
    struct RenderData {
        static constexpr float ACTIVE_SPREAD = 1.0f;
        static constexpr float INACTIVE_SPREAD = -1.0f;
        glm::mat4 modelMatrix;

        float time;
        float active;
        glm::vec2 padding;
    };

    enum AddSpreadDistribution {
        AD_Constant,
        AD_Feather
    };

    static constexpr float SPREAD_DIST = 2.0f;
    static constexpr int MAX_SPREAD = 32768;
    static constexpr int KEY_LENGTH = 1024;
    static constexpr int MAX_SPREAD_PARTICLES = 4096;

    bool tramples_[KEY_LENGTH][KEY_LENGTH];
    vector_contig<RenderData, MAX_SPREAD> spreadData_[SpreadType_Num];
    KeyIndex keys_[KEY_LENGTH][KEY_LENGTH];

    SpreadManager(
        SeedManager& seedManager,
        Terrain& terrain
    );
    SpreadManager(const SpreadManager&) = delete;

    bool SpreadIsActive(const glm::vec2& position) const;
    bool SpreadIsActive(const glm::vec3& position) const;

    bool AddSpread(const glm::vec3& position, SpreadType type = SpreadType_Flower); 
    bool AddSpread(const SpreadKey& key, SpreadType type = SpreadType_Flower); 
    AddSpreadInfo AddSpread(
        const glm::vec3& position, 
        int radius, 
        float density = 1.0f, 
        AddSpreadDistribution distribution = AD_Constant, 
        SpreadType type = SpreadType_Flower
    );
    AddSpreadInfo AddSpread(
        const glm::vec3& position, 
        float radius, 
        float density = INT_MAX, 
        AddSpreadDistribution distribution = AD_Constant, 
        SpreadType type = SpreadType_Flower
    );

    bool RemoveSpread(
        const SpreadKey& key,
        Entity* remover = nullptr,
        bool deactivate = false
    );
    bool RemoveSpread(
        const glm::vec3& position, 
        Entity* remover = nullptr,
        bool deactivate = false
    );
    int RemoveSpread(
        const glm::vec3& position, 
        int radius, 
        Entity* remover = nullptr,
        bool deactivate = false
    ); 
    int RemoveSpread(
        const glm::vec3& position, 
        float radius, 
        Entity* remover = nullptr,
        bool deactivate = false
    ); 
    int RemoveSpread(
        Cone& cone, 
        Entity* remover = nullptr,
        bool deactivate = false
    ); 

    void ClearTramples();
    bool Trample(const SpreadKey& key, Entity* trampler = nullptr);
    bool Trample(const glm::vec3& position, Entity* trampler = nullptr);
    int Trample(const glm::vec3& position, float radius, Entity* trampler = nullptr);

    void UpdateRenderData_P();

    SpreadKey GetKey(const glm::vec2& position) const;
    SpreadKey GetKey(const glm::vec3& position) const;
    glm::vec2 KeyToPosition(const SpreadKey& key) const;

    void Reset();
    int GetCount(SpreadType type) const { return spreadData_[type].size(); }

private:
    Terrain& terrain_;
    SeedManager& seedManager_;
};
