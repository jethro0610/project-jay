#pragma once
#include <bitset>
#include <string>
#include "Component.h"
#include "../../Helpers/Assert.h"
#include "../../Rendering/Renderer.h"
#include "../../Rendering/Skeleton.h"

class SkeletonComponent : public Component {
public:
    Renderer* renderer;

    std::array<Skeleton*, MAX_ENTITIES> skeleton;
    std::array<Pose, MAX_ENTITIES> pose; // OPTIMIZATION: Can reserve Poses in Renderer then request a pointer
    std::array<Pose, MAX_ENTITIES> prevPose;
    std::array<Pose, MAX_ENTITIES> renderPose; 

    std::array<int, MAX_ENTITIES> prevAnimationIndex;
    std::array<int, MAX_ENTITIES> nextAnimationIndex;
    std::array<int, MAX_ENTITIES> animationIndex;

    std::array<float, MAX_ENTITIES> prevTime; 
    std::array<float, MAX_ENTITIES> time;
    std::array<float, MAX_ENTITIES> transitionTime;
    std::array<float, MAX_ENTITIES> transitionLength;
    std::array<bool, MAX_ENTITIES> transitionThisTick;

    std::array<float, MAX_ENTITIES> nextPoseUpdate; 

    SkeletonComponent() {
        skeleton.fill(nullptr);
        prevAnimationIndex.fill(0);
        animationIndex.fill(0);
        time.fill(0.0f);
        transitionTime.fill(0.0f);
        nextPoseUpdate.fill(0.0f);
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
    }
};