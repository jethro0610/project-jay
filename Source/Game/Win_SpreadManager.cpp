#include "SpreadManager.h"

SpreadManager::SpreadManager(DXResources* dxResources) {
    count_ = 0;
    dirty_ = false;
    dxResources_ = dxResources; 
    dxResources_->CreateInstanceBuffer(&instanceBuffer_);
}

void SpreadManager::UpdateRenderData_P() {
    if (!dirty_)
        return;

    dxResources_->UpdateBuffer(instanceBuffer_, positions_, sizeof(positions_));
}
