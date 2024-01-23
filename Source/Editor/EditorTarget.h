#pragma once
#include "Text/Text.h"
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

class Entity;
class TerrainBubble;
class TerrainCurve;
class EntityList;

class EditorTarget {
public:
    EditorTarget();
    void Untarget();
    void SetEntity(Entity* target);
    void SetCurve(TerrainCurve* target, int point);
    void SetBubble(TerrainBubble* target);
    bool IsEntity() { return entity_ != nullptr; }
    bool IsTerrainControl() { return bubble_ != nullptr || curve_ != nullptr; }
    bool HasTarget() { return entity_ != nullptr | bubble_ != nullptr | curve_ != nullptr; }; 

    Entity* GetEntity() { return entity_; };
    
    glm::vec3 GetPosition();
    void SetPosition(glm::vec3 pos);

    glm::vec3 GetScale();
    void SetScale(glm::vec3 scale);

    glm::quat GetRotation();
    void SetRotation(glm::quat rotation);

    Text name_;

private:
    Entity* entity_; 
    TerrainBubble* bubble_;
    TerrainCurve* curve_;
    int curvePoint_;
};
