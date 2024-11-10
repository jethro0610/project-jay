#pragma once
#include "Text/Text.h"
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

class Entity;
class TerrainBubble;
class TerrainCurve;
class TerrainNoise;
class EntityList;

class EditorTarget {
public:
    EditorTarget();
    void Untarget();
    void SetEntity(Entity* target);
    void SetCurve(TerrainCurve* target, int point);
    void SetBubble(TerrainBubble* target);
    void SetNoise(TerrainNoise* noise);
    void Destroy();
    bool IsEntity() { return entity_ != nullptr; }
    bool IsTerrainControl() { return bubble_ != nullptr || curve_ != nullptr || noise_ != nullptr; }
    bool HasTarget() { return entity_ != nullptr || bubble_ != nullptr || curve_ != nullptr || noise_ != nullptr; }; 

    Entity* GetEntity() { return entity_; };
    
    glm::vec3 GetPosition();
    void SetPosition(glm::vec3 pos);

    glm::vec4 GetScale();
    void SetScale(glm::vec4 scale);

    glm::quat GetRotation();
    void SetRotation(glm::quat rotation);

    Text name_;

private:
    Entity* entity_; 
    TerrainBubble* bubble_;
    TerrainCurve* curve_;
    TerrainNoise* noise_;
    int curvePoint_;
};
