#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include "Entity/EntityProperties.h"

class Entity;
class EditorTargetController;
class Ray;
class Terrain;

class EditorTarget {
private:
    friend EditorTargetController;

protected:
    bool selected_;

public:
    EditorTarget() : selected_(false) {}
    virtual std::string GetName() = 0;

    virtual glm::vec3 GetPosition() { return glm::vec3(0.0f); };
    virtual void SetPosition(const glm::vec3& pos) {};

    virtual glm::quat GetRotation() { return glm::quat(0.0f, 0.0f, 0.0f, 0.0f); };
    virtual void SetRotation(const glm::quat& rot) {};

    virtual glm::vec4 GetScale() { return glm::vec4(0.0f); }
    virtual void SetScale(const glm::vec4& ref, const glm::vec4& delta) {};

    virtual EntityProperties GetProperties() { return EntityProperties(); };

    virtual bool RayHit(const Ray& ray) = 0;

    virtual EditorTarget* Clone() { return nullptr; };

    virtual void Destroy() {};

    virtual bool TerrainAlignable() { return false; }
    virtual bool IsEntity() { return false; }
    virtual bool UpdateTerrain() { return false; }
    virtual bool Selectable() { return true; }
    virtual bool Clonable() { return false; }
    bool Selected() { return selected_; }
};
