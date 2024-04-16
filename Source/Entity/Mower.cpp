#include "Mower.h"
#include "Resource/ResourceManager.h"
#include "Terrain/Terrain.h"
#include "Spread/SpreadManager.h"
#include "EntityList.h"
using namespace glm;

EntityDependendies Mower::GetDeps() {
    return {
        "st_tpillar"
    };
}

void Mower::Init(Entity::InitArgs args) {
    Entity::Init(args);

    SetFlag(EF_GroundCheck, true);
    SetFlag(EF_StickToGround, true);
    SetFlag(EF_AlignToGround, true);
    SetFlag(EF_Interpolate, true);
    SetFlag(EF_UseVelocity, true);

    ResourceManager& resourceManager = args.resourceManager;
    model_ = resourceManager.GetModel("st_tpillar");
    materials_[0].shader = resourceManager.GetShader("vs_static", "fs_dfsa_color");
    materials_[0].shadowShader = resourceManager.GetShader("vs_static_s", "fs_depth_s");
    materials_[0].castShadows = true;
    materials_[0].properties.color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

    pushbox_.top = 1.0f;
    pushbox_.bottom = 1.0f;
    pushbox_.radius = 1.0f;
}

void Mower::Update() {
    static const float MAX_DIST = 10.0f;

    Entity& player = (*entities_)[0];
    vec3 dirToPlayer = player.transform_.position - transform_.position;
    dirToPlayer.y = 0.0f;
    if (length(dirToPlayer) > 20.0f) {
        float prevY = transform_.position.y;
        transform_.position = player.transform_.position - normalize(dirToPlayer) * 20.0f;
        transform_.position.y = prevY;
    }
    velocity_.y -= 1.0f;

    spreadManager_->RemoveSpread(transform_.position, 5, this);
}
