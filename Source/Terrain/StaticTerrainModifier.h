#pragma once
#include <nlohmann/json.hpp>
#include "Types/Transform.h"
#include "Editor/EditorTarget.h"

class StaticTerrainModifier {
public:
    static constexpr int MAX = 128 * 3;
    static constexpr int ID = 0;
    StaticTerrainModifier() {
        id_ = StaticTerrainModifier::ID;
        active_ = false;
        destroy_ = false;
        editorTargets_.clear();
    }

    int id_;
    char label_[32];
    bool active_;
    bool destroy_;
    vector_const<EditorTarget*, 8> editorTargets_;
    Terrain* terrain_;

    struct RenderNode {
        static constexpr int MAX = 64;
        Transform transform;
        bool selected;
        glm::vec4 color;
    };

    float GetHeight(const glm::vec2& pos);
    bool InRange(const glm::vec2& pos);
    void Save(nlohmann::json& json);
    void Load(const nlohmann::json& json);
    void WriteRenderNodes(vector_const<RenderNode, RenderNode::MAX>& nodes, Terrain& terrain);
    static int GetIDFromName(const std::string& name);
    std::string GetName();
};
