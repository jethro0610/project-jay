#include "PlayerInputSystem.h"
using namespace glm;

void PlayerInputSystem::Execute(Inputs inputs, Camera* camera, Entity* entities, InputComponent& inputComponent) {
    assert(inputComponent.recieveFrom[PLAYER_ENTITY] == RecieveInput::Player);

    quat cameraPlanarRotation = quat(vec3(0.0f, camera->lookX_, 0.0f));
    vec3 cameraPlanarForward = cameraPlanarRotation * Transform::worldForward;
    vec3 cameraPlanarRight = cameraPlanarRotation * Transform::worldRight;

    vec3 desiredMovement = cameraPlanarForward * inputs.forwardInput + cameraPlanarRight * inputs.sideInput;
    if (length(desiredMovement) > 1.0f)
        desiredMovement = normalize(desiredMovement);

    inputComponent.direction[PLAYER_ENTITY] = desiredMovement;

    inputComponent.toggle[PLAYER_ENTITY] = inputs.ski;
    inputComponent.action[PLAYER_ENTITY] = inputs.pickup;
}
