#include "SpreadManager.h"

SpreadManager::SpreadManager(DXResources* dxResources) {
    count_ = 0;
    dirty_ = false;
    dxResources_ = dxResources; 
    dxResources_->CreateInstanceBuffer(&instanceBuffer_);
}
