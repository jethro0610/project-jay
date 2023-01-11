#include "../Entity/Entity.h"
#include "../Components/HitboxComponent.h"
#include "../Components/TransformComponent.h"

namespace KickSystem {
    void Execute(
        Entity* entities,
        TransformComponent& transformComponent,
        HitboxComponent& hitboxComponent
    );
};
