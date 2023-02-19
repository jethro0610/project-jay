#include "../Entity/Entity.h"
#include "../SpreadManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/BubbleComponent.h"

#include "../Components/KickerComponent.h"
#include "../Components/KickableComponent.h"

#include "../Components/PickupComponent.h"
#include "../Components/HoldableComponent.h"

namespace IntersectSystem {
    void Execute(
        Entity* entities,
        TransformComponent& transformComponent,
        BubbleComponent& bubbleComponent,
        PickupComponent& pickupComponent,
        HoldableComponent& holdableComponent,
        KickerComponent& kickerComponent,
        KickableComponent& kickableComponent
    ); 

    void HandleIntersection(
        Entity* entites,
        int hitbox1,
        int hitbox2,
        PickupComponent& pickupComponent,
        HoldableComponent& holdableComponent,
        KickerComponent& kickerComponent,
        KickableComponent& kickableComponent
    );
};
