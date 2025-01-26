#include "StaticTerrainModifier.h"
#include "TerrainModifiers.h"
#include "Helpers/Assert.h"

float StaticTerrainModifier::GetHeight(const glm::vec2& pos) {
    ASSERT((id_ != 0), "Unassigned terrain modifier ID");
    switch (id_) {
        #define TERRAINMOD(TYPE, ARR) case TYPE::ID: return ((TYPE*)this)->GetHeight(pos); break;
        EXPANDTERRAINMODS 
        #undef TERRAINMOD
        
        default:
            return 0.0;
    }
}

bool StaticTerrainModifier::InRange(const glm::vec2& pos) {
    ASSERT((id_ != 0), "Unassigned terrain modifier ID");
    switch (id_) {
        #define TERRAINMOD(TYPE, ARR) case TYPE::ID: return ((TYPE*)this)->InRange(pos); break;
        EXPANDTERRAINMODS 
        #undef TERRAINMOD

        default:
            return false;
    }
}

void StaticTerrainModifier::Save(nlohmann::json& json) {
    ASSERT((id_ != 0), "Unassigned terrain modifier ID");
    switch (id_) {
        #define TERRAINMOD(TYPE, ARR) case TYPE::ID: ((TYPE*)this)->Save(json); break;
        EXPANDTERRAINMODS 
        #undef TERRAINMOD

        default:
            break;
    }
}

void StaticTerrainModifier::Load(const nlohmann::json& json) {
    ASSERT((id_ != 0), "Unassigned terrain modifier ID");
    switch (id_) {
        #define TERRAINMOD(TYPE, ARR) case TYPE::ID: ((TYPE*)this)->Load(json); break;
        EXPANDTERRAINMODS 
        #undef TERRAINMOD

        default:
            break;
    }
}

void StaticTerrainModifier::WriteRenderNodes(vector_const<RenderNode, RenderNode::MAX>& nodes, Terrain& terrain) {
    ASSERT((id_ != 0), "Unassigned terrain modifier ID");
    switch (id_) {
        #define TERRAINMOD(TYPE, ARR) case TYPE::ID: ((TYPE*)this)->WriteRenderNodes(nodes, terrain); break;
        EXPANDTERRAINMODS 
        #undef TERRAINMOD

        default:
            break;
    }
}
