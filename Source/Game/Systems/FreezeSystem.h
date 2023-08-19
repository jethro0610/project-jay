#include <array>
#include "../Entity/EntityLimits.h"
class Entity;

namespace FreezeSytem {
    void Execute(std::array<Entity, MAX_ENTITIES>& entities);
}
