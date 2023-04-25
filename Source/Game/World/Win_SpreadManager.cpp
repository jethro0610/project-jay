#include "SpreadManager.h"
#include "../../Helpers/ChunkHelpers.h"
#include "../../Resource/DXResources.h"
#include "../../Resource/ResourceManager.h"
using namespace glm;

void SpreadManager::UpdateRenderData_P() {
    DXResources& dxResources = resourceManager_.dxResources_;

    for (auto it = dirtyChunks_.begin(); it != dirtyChunks_.end(); it++) {
        ivec2 chunk = *it;
        chunk = GetNormalizedChunk2D(chunk);
        SpreadChunk& spreadChunk = spreadChunks_[chunk.x][chunk.y];
        dxResources.UpdateBuffer(dxResources.spreadBuffers_[chunk.x][chunk.y], spreadChunk.positions, sizeof(vec3) * MAX_SPREAD); 
    }
    dirtyChunks_.clear();
}
