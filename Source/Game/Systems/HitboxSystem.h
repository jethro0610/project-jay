#include "../Entity/Entity.h"
#include "../SpreadManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/HitboxComponent.h"
#include "../Components/BubbleComponent.h"

namespace HitboxSystem {
    void Execute(
        Entity* entities,
        TransformComponent& transformComponent,
        HitboxComponent& hitboxComponent,
        BubbleComponent& bubbleComponent
    ); 

    void HandleIntersection(
        int hitbox1,
        int hitbox2,
        HitboxComponent& hitboxComponent
    );
};
