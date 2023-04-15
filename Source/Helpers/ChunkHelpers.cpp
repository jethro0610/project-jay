#include "ChunkHelpers.h"
#include "../Constants/WorldConstants.h"
using namespace glm;

ivec3 GetChunkAtWorldPosition(vec3 position) {
    return ivec3(floor(position / CHUNK_SIZE));
}

ivec2 GetChunkAtWorldPosition2D(vec2 position) {
    return ivec2(floor(position / CHUNK_SIZE));
}

ivec3 GetNormalizedChunk(ivec3 chunk) {
    chunk.x += MAX_X_CHUNKS / 2;
    chunk.y += MAX_Y_CHUNKS / 2;
    chunk.z += MAX_Z_CHUNKS / 2;
    return chunk;
}

ivec2 GetNormalizedChunk2D(ivec2 chunk) {
    chunk.x += MAX_X_CHUNKS / 2;
    chunk.y += MAX_Y_CHUNKS / 2;
    return chunk;
}

