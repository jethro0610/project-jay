#include "ProjectileSystem.h"
using namespace glm;

void ProjectileSystem::CalculateVelocities(
    EntityManager& entityManager,
    World& world,
    ProjectileComponent& projectileComponent,
    VelocityComponent& velocityComponent,
    TransformComponent& transformComponent
) {
    const vec3& playerPosition = transformComponent.transform[PLAYER_ENTITY].position_;
    const vec3& playerVelocity = velocityComponent.velocity[PLAYER_ENTITY];
    for (int i = 0; i < MAX_ENTITIES; i++) {
        const Entity& entity = entityManager.entities_[i];
        if (!entity.alive_)
            continue;
        
        if (!entity.HasComponents({
            projectileComponent, 
            velocityComponent,
            transformComponent
        })) continue;

        ProjectileState& state = projectileComponent.state[i];
        if (state == Inactive)
            continue;
        const float& param1 = projectileComponent.param1[i];
        const float& param2 = projectileComponent.param2[i];
        vec3& velocity = velocityComponent.velocity[i];
        vec3& position = transformComponent.transform[i].position_;

        // Gravity
        if (state == ProjectileState::Launch || state == ProjectileState::Throw)
            velocity.y -= 1.0f;

        // Rise
        if (position.y >= 200.0f && state == ProjectileState::Rise) {
            velocity.y = -param2;

            position.x = playerPosition.x;
            position.z = playerPosition.z;
            state = ProjectileState::Fall;
        }

        // Fall
        if (abs(position.y - playerPosition.y) >= 50.0f && state == ProjectileState::Fall) {
            position.x = playerPosition.x;
            position.z = playerPosition.z;
        }

        // Ground inactivate
        // TODO: Move this to generic world intersect component
        const float distance = world.GetDistance(position);
        if (distance < 0.2f) {
            projectileComponent.state[i] = ProjectileState::Inactive;
            velocity = vec3(0.0f);
            position.y -= distance;
        }
    }
}

void ProjectileSystem::Launch(
    ProjectileComponent& projectileComponent,
    VelocityComponent& velocityComponent,
    TransformComponent& transformComponent,
    EntityID projectileEntity
) {
    const float& param1 = projectileComponent.param1[projectileEntity];
    const float& param2 = projectileComponent.param2[projectileEntity];
    vec3& velocity = velocityComponent.velocity[projectileEntity];

    switch (projectileComponent.type[projectileEntity]) {
        case ProjectileType::Random: {
            velocity.y = param2;
            const int randAngle = rand();
            const float randSpeed = (rand() % (int)(param1 * 100)) * 0.01f;
            const float x = cos(randAngle); 
            const float y = sin(randAngle); 
            velocity.x = x * randSpeed;
            velocity.z = y * randSpeed;
            projectileComponent.state[projectileEntity] = ProjectileState::Launch;
            break;
         }
    
        case ProjectileType::RiseAndFall: {
            velocity.y = param1;
            projectileComponent.state[projectileEntity] = ProjectileState::Rise;
            break;
        }

        default:
            break;
    }
}

void ProjectileSystem::Throw(
    ProjectileComponent& projectileComponent,
    VelocityComponent& velocityComponent,
    TransformComponent& transformComponent,
    EntityID projectileEntity,
    EntityID throwingEntity,
    float height
) {
    vec3& velocity = velocityComponent.velocity[projectileEntity]; 
    velocity = vec3(0.0f, 30.0f, 0.0f);
    const vec3& throwerVelocity = velocityComponent.velocity[throwingEntity];
    velocity = vec3(throwerVelocity.x, height, throwerVelocity.z);
    transformComponent.transform[projectileEntity].position_.y += 0.5f;
    projectileComponent.state[projectileEntity] = ProjectileState::Throw;
}
