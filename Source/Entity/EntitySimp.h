#pragma once
#include "Types/Transform.h"
#include "Rendering/Model.h"
#include "Rendering/Skeleton.h"
#include "Rendering/Material.h"
#include <vector_const.h>

class EntitySimp {
public:
    static constexpr int MAX_NAME = 32;

    Transform transform_;
    Transform lastTransform_;
    Model* model_;
    vector_const<Material*, Model::MAX_MESHES_PER_MODEL> materials_;

    #ifdef _DEBUG
    bool DBG_selected_;
    vector_const<Material*, Model::MAX_MESHES_PER_MODEL> DBG_selectMaterials_;
    virtual char* GetName() const = 0;
    #endif

    virtual void Update() = 0;
    virtual bool Interpolate() { return false; }
};
