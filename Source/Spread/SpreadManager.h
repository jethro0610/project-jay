#pragma once
#include "SpreadType.h"
#include "Types/Cone.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <vector_contig.h>
#include <unordered_map>

class Entity;
class Terrain;
class SeedManager;
class TransformComponent;

struct AddSpreadInfo {
    int count;
    glm::ivec2 spreadSpacePos;
};

class SpreadManager {
public:
    static constexpr int CHUNK_SIZE = 64;
    static constexpr float SPREAD_DIST = 2.0f;
    static constexpr float CHUNK_LENGTH = CHUNK_SIZE * SPREAD_DIST;
    static constexpr int MAX_SPREAD = 1048576; // 2^20
    static constexpr int NUM_CHUNKS = MAX_SPREAD / (CHUNK_LENGTH * CHUNK_LENGTH);

    struct RenderData {
        static constexpr float ACTIVE_SPREAD = 1.0f;
        static constexpr float INACTIVE_SPREAD = -1.0f;
        glm::mat4 modelMatrix;

        float time;
        float active;
        glm::vec2 padding;
    };
    struct Chunk {
        bool active;
        glm::vec2 origin;
        vector_contig<RenderData, CHUNK_SIZE * CHUNK_SIZE> renderData[SpreadType_Num];
        int indexes[CHUNK_SIZE][CHUNK_SIZE];
        SpreadType types[CHUNK_SIZE][CHUNK_SIZE];
    };
    struct ChunkSpacePosition {
        glm::ivec2 chunk;
        glm::ivec2 spread;
    };

    Chunk chunks_[NUM_CHUNKS];
    std::unordered_map<glm::ivec2, int> chunkIndexes_;

    SpreadManager(
        SeedManager& seedManager,
        Terrain& terrain
    );
    SpreadManager(const SpreadManager&) = delete;

    static glm::ivec2 WorldSpaceToSpreadSpace(const glm::vec3& worldSpacePos);
    static ChunkSpacePosition SpreadSpaceToChunkSpace(const glm::ivec2& spreadSpacePos);
    static glm::ivec2 ChunkSpaceToSpreadSpace(const ChunkSpacePosition& chunkSpacePos);
    static glm::vec3 SpreadSpaceToWorldSpace(const glm::ivec2& spreadSpacePos);

    //bool HasSpread(const glm::vec3& worldSpacePos) const;

    enum AddSpreadDistribution {
        AD_Constant,
        AD_Feather
    };
    bool AddSpread(const glm::ivec2& spreadSpacePos, SpreadType type = SpreadType_Flower); 
    bool AddSpread(const glm::vec3& worldSpacePos, SpreadType type = SpreadType_Flower); 
    AddSpreadInfo AddSpread(
        const glm::vec3& worldSpacePos, 
        int radius, 
        float density = 1.0f, 
        AddSpreadDistribution distribution = AD_Constant, 
        SpreadType type = SpreadType_Flower
    );
    AddSpreadInfo AddSpread(
        const glm::vec3& worldSpacePos, 
        float radius, 
        float density = INT_MAX, 
        AddSpreadDistribution distribution = AD_Constant, 
        SpreadType type = SpreadType_Flower
    );

    bool RemoveSpread(
        const glm::ivec2& spreadSpacePos,
        Entity* remover = nullptr,
        bool deactivate = false
    );
    bool RemoveSpread(
        const glm::vec3& worldSpacePos, 
        Entity* remover = nullptr,
        bool deactivate = false
    );
    int RemoveSpread(
        const glm::vec3& worldSpacePos, 
        int radius, 
        Entity* remover = nullptr,
        bool deactivate = false
    ); 
    int RemoveSpread(
        const glm::vec3& worldSpacePos, 
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
    bool Trample(const glm::ivec2& spreadSpacePos, Entity* trampler = nullptr);
    bool Trample(const glm::vec3& worldSpacePos, Entity* trampler = nullptr);
    int Trample(const glm::vec3& worldSpacePos, float radius, Entity* trampler = nullptr);

    void UpdateRenderData_P();

    void Reset();

private:
    Terrain& terrain_;
    SeedManager& seedManager_;
};
