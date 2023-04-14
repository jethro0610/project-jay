#include "ChunkHelpers.h"
#include "../Constants/WorldConstants.h"
using namespace glm;

glm::ivec2 WorldToChunkPosition2D(glm::vec2 position) {
    return ivec2(floor(position.x / CHUNK_SIZE), floor(position.y / CHUNK_SIZE));
}
