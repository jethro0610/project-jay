#include "ProjectileSystem.h"
using namespace glm;

void ProjectileSystem::CalculateVelocities(
    EntityManager& entityManager,
    World& world,
    ProjectileComponent& projectileComponent,
    VelocityComponent& velocityComponent,
    TransformComponent& transformComponent
) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        if (!entity.alive_)
            continue;
        
        if (!entity.HasComponents<
            ProjectileComponent, 
            VelocityComponent,
            TransformComponent
        >()) continue;

        if (!projectileComponent.active[i])
            continue;

        // Gravity
        vec3& velocity = velocityComponent.velocity[i];
        velocity.y -= 1.0f;

        const float distance = world.GetDistance(transformComponent.transform[i].position_);
        if (distance < 0.2f) {
            projectileComponent.active[i] = false;
            velocity = vec3(0.0f);
        }
    }
}

void ProjectileSystem::Launch(
    uint16_t projectileEntity, 
    uint16_t targetEntity,
    glm::vec3 velocity,
    ProjectileComponent& projectileComponent,
    VelocityComponent& velocityComponent,
    TransformComponent& transformComponent
) {
    vec3 planarVelocity = vec3(velocity.x, 0.0f, velocity.z) * 0.99f;
    velocityComponent.velocity[projectileEntity] = vec3(planarVelocity.x, 30.0f, planarVelocity.z);
    projectileComponent.active[projectileEntity] = true;
    transformComponent.transform[projectileEntity].position_.y += 1.0f;
}
