#include "WaterManager.h"
#include "Entity/EntityList.h"
#include "Entity/Entity.h"
using namespace glm;

WaterManager::WaterManager(EntityList& entityList) :
    entities_(entityList) 
{

}

void WaterManager::Wet(const vec2& position, float area) {
    float area2 = area * area;
    for (int i = 0; i < EntityList::MAX; i++) {
        Entity& entity = entities_[i];
        if (!entity.alive_) continue;

        vec2 entityPosition = vec2(entity.transform_.position.x, entity.transform_.position.z);
        if (distance2(position, entityPosition) < area2)
            entity.DoWet();
    }
}

void WaterManager::Wet(const vec3& position, float area) {
    Wet(vec2(position.x, position.z), area);
}
