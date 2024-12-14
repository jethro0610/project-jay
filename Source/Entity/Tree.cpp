#include "Tree.h"
#include "Resource/ResourceManager.h"
#include "Entity/EntityList.h"
#include "Apple.h"
#include "Helpers/Random.h"
#include "Terrain/Terrain.h"
#include "Time/Time.h"
using namespace glm;

EntityDependendies Tree::GetStaticDependencies() {
    return {
        "st_tpillar"
    };
}

void Tree::Init(Entity::InitArgs args) {
    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = vec4(0.5f, 1.0f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;

    SetFlag(EF_RecieveHits, true);
    SetFlag(EF_SendPush, true);
    hurtbox_.radius = 1.5f;
}

void Tree::Start() {
    for (int i = 0; i < MAX_APPLES; i++) {
        Transform appleTransform;
        appleTransform.position = 
            transform_.position + 
            vec3(0.0f, transform_.scale.y + 5.0f, 0.0f) + 
            RandomVector(2.0f, 4.0f) * transform_.scale.x;

        appleTransform.scale = vec3(RandomFloatRange(4.0, 6.0));
        apples_[i] = (Apple*)&entities_->CreateEntity(Apple::TYPEID, appleTransform);
        apples_[i]->active_ = false;
        apples_[i]->growth_ = 0.0f;
    }
}


void Tree::Update() {

}

void Tree::OnHurt(HurtArgs args) {
    for (int i = 0; i < MAX_APPLES; i++) {
        if (apples_[i] == nullptr)
            continue;

        apples_[i]->active_ = true;
        const vec3& applePosition = apples_[i]->transform_.position;

        vec3 point = terrain_->GetRandomPointInSameIsland(transform_.position, 50.0f, 800.0f);
        float jumpStrength = RandomFloatRange(10.0f, 30.0f);

        // Quadratic equation to get airtime
        static constexpr float A = -0.5 * Apple::GRAVITY * GlobalTime::TIMESTEP;
        float b = jumpStrength * GlobalTime::TIMESTEP;
        float bSqr = b * b;
        float c = applePosition.y - point.y;
        float airtime = ((-b - sqrt(bSqr - 4 * A * c)) / (2 * A)) * GlobalTime::TIMESTEP;

        // Get the direction and distance to the point
        vec3 planarApplePosition = applePosition;
        planarApplePosition.y = 0.0f;
        vec3 planarPointPosition = point;
        planarPointPosition.y = 0.0f;
        float distanceToPoint = distance(planarApplePosition, planarPointPosition);
        vec3 directionToPoint = normalize(planarPointPosition - planarApplePosition);

        // Then get the required magnitude based on the airtime
        float magToPoint = (distanceToPoint / airtime);

        apples_[i]->velocity_ = directionToPoint * magToPoint + vec3(0.0f, jumpStrength, 0.0f);
        apples_[i] = nullptr;
    }

    for (int i = 0; i < MAX_APPLES; i++) {
        if (apples_[i] != nullptr)
            continue;

        for (int j = i + 1; j < MAX_APPLES; j++) {
            if (apples_[j] == nullptr)
                continue;

            apples_[i] = apples_[j];
            apples_[j] = nullptr;
            break;
        }
    }
}
