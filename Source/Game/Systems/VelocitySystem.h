#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"

namespace VelocitySystem {
    void Apply(
        Entity* entities, 
        TransformComponent& transformComponent,
        VelocityComponent& velocityComponent
    );
}
