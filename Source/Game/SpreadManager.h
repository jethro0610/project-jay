#pragma once
#include "../Types/Transform.h"
#include <unordered_map>
#include <glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/hash.hpp"

#ifdef _WINDOWS
#include "../Resource/DXResources.h"
#endif

class SpreadManager {
public:
    #ifdef _WINDOWS
    SpreadManager(DXResources* dxResources);
    DXResources* dxResources_;
    ID3D11Buffer* instanceBuffer_;
    #endif

    std::unordered_map<glm::ivec2, uint16_t> keys_; 
    uint16_t count_;
    glm::vec3 positions_[MAX_INSTANCES];
    bool dirty_;

    void AddSpread(glm::ivec2 key); 
    void UpdateRenderData_P();
};
