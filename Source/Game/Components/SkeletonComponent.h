#pragma once
#include <bitset>
#include <string>
#include "Component.h"
#include "../../Helpers/Assert.h"
#include "../../Rendering/Renderer.h"
#include "../../Rendering/Skeleton.h"

const int ANIM_STATES = 32;
namespace AnimState {
    const int IDLE = 0;
    const int WALK = 1;
    const int SKI = 2;
    const int FLOW = 3;
}

class SkeletonComponent : public Component {
public:
    Renderer* renderer;

    std::array<Skeleton*, MAX_ENTITIES> skeleton;
    std::array<int, MAX_ENTITIES> animState;
    std::array<std::array<int, ANIM_STATES>, MAX_ENTITIES> stateBindings;

    std::array<Pose, MAX_ENTITIES> pose; // OPTIMIZATION: Can reserve Poses in Renderer then request a pointer
    std::array<Pose, MAX_ENTITIES> prevPose;
    std::array<Pose, MAX_ENTITIES> renderPose; 

    std::array<float, MAX_ENTITIES> time; 
    std::array<float, MAX_ENTITIES> nextPoseUpdate; 

    SkeletonComponent() {
        skeleton.fill(nullptr);
        time.fill(0.0f);
        nextPoseUpdate.fill(0.0f);
        animState.fill(0);
        memset(stateBindings.data(), 0, sizeof(int) * ANIM_STATES * MAX_ENTITIES); 
    };

    SkeletonComponent(const SkeletonComponent&) = delete;
    SkeletonComponent& operator=(const SkeletonComponent&) = delete;

    static constexpr std::string GetName() { return "skeleton"; }
    static constexpr int GetID() { return 14; }

    void Load(nlohmann::json& data, EntityID entity) {
        std::string name = GetString(data, "skeleton", "null_skeleton");

        skeleton[entity] = &renderer->GetSkeleton(name);
        pose[entity].resize(skeleton[entity]->bones_.size());
        renderPose[entity].resize(skeleton[entity]->bones_.size());
        prevPose[entity].resize(skeleton[entity]->bones_.size());

        auto bindings = data["state_bindings"];
        for (auto& binding : bindings)
            stateBindings[entity][binding[0]] = binding[1]; 
    }
};
