#include "../Entity/Entity.h"
#include "../SpreadManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/HitboxComponent.h"

namespace HitboxSystem {
    void Execute(
        Entity* entities,
        TransformComponent& transformComponent,
        HitboxComponent& hitboxComponent
    ); 
};