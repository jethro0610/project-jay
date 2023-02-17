#include "../Entity/Entity.h"
#include "../SpreadManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/HitboxComponent.h"

namespace KickSystem {
    void Execute(
        Entity* entities,
        SpreadManager* spreadManager,
        TransformComponent& transformComponent,
        HitboxComponent& hitboxComponent
    ); 
};
