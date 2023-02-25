#include "../Entity/EntityManager.h"
#include "../SpreadManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/BubbleComponent.h"

#include "../Components/KickerComponent.h"
#include "../Components/KickableComponent.h"

#include "../Components/PickupComponent.h"
#include "../Components/HoldableComponent.h"

namespace IntersectSystem {
    void Execute(
        EntityManager& entityManager,
        SpreadManager& spreadManager,
        TransformComponent& transformComponent,
        BubbleComponent& bubbleComponent,
        PickupComponent& pickupComponent,
        HoldableComponent& holdableComponent,
        KickerComponent& kickerComponent,
        KickableComponent& kickableComponent
    ); 

    void HandleIntersection(
        EntityManager& entityManager,
        SpreadManager& spreadManager,
        int hitbox1,
        int hitbox2,
        TransformComponent& transformComponent,
        PickupComponent& pickupComponent,
        HoldableComponent& holdableComponent,
        KickerComponent& kickerComponent,
        KickableComponent& kickableComponent
    );
};
