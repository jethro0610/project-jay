#include "SpreadManager.h"

void SpreadManager::UpdateRenderData_P() {
    if (!dirty_)
        return;
    
    DXResources* dxResources = resourceManager_->dxResources_;
    dxResources->UpdateBuffer(dxResources->spreadBuffer_, positions_, sizeof(positions_));
}
