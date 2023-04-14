#include "SpreadManager.h"

void SpreadManager::UpdateRenderData_P() {
    DXResources& dxResources = resourceManager_.dxResources_;

    for (auto it = dirtyChunks_.begin(); it != dirtyChunks_.end(); it++) {
        SpreadChunk& chunk = chunks_[it->x][it->y];
        dxResources.UpdateBuffer(dxResources.spreadBuffers_[it->x][it->y], chunk.positions, sizeof(vec3) * MAX_SPREAD); 
    }
    dirtyChunks_.clear();
}
