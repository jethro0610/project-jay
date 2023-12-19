#include "CursorMode.h"
#include "Collision/Ray.h"
#include "Rendering/Renderer.h"
using namespace glm;

CursorMode::CursorMode(EditorModeArgs args):
EditorMode(args)
{
    mouseVisibile_ = true;
}

vec3 CursorMode::GetMouseRay() {
    float mouseX = platform_.mouseX_ / (1280  * 0.5f) - 1.0f;
    float mouseY = platform_.mouseY_ / (720 * 0.5f) - 1.0f;

    glm::mat4 invVP = glm::inverse(renderer_.GetProjectionMatrix() * camera_.GetViewOnlyMatrix());
    glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
    glm::vec4 worldPos = invVP * screenPos;

    glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

    return dir;
}

void CursorMode::Update() {
    TransformComponent& transformComponent = entityManager_.components_.Get<TransformComponent>();
    if (platform_.pressedKeys_[LEFT_MOUSE_KEY]) {
        target_.Set(NULL_ENTITY);
        float maxDist = INFINITY;
        vec3 mouseRay = GetMouseRay();
        for (int i = 0; i < MAX_ENTITIES; i++) {
            const Entity& entity = entityManager_.entities_[i];
            if (!entity.alive_) continue;

            Transform& transform = transformComponent.transform[i];
            float dist = distance(camera_.transform_.position, transform.position);
            if (
                Ray::RayHitCollider(camera_.transform_.position, mouseRay, transform, entity.DBG_collider) &&
                dist < maxDist
            ) {
                maxDist = dist;
                target_.Set(i);
            }
        }
    }
}
