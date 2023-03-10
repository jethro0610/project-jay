#pragma once
#include "Entity.h"
#include "../../Resource/ResourceManager.h"
#include "../Components/ComponentInclude.h"
#include "../../Logging/Logger.h"
#include <bitset>
#include <tuple>
#include <unordered_map>
#include <deque>
#include <string>

template <class... ComponentTypes>
class EntityManagerTemp {
public:
    Entity entities_[MAX_ENTITIES];
    std::deque<uint16_t> usableEntities_;

    EntityManagerTemp (ResourceManager& resourceManager):
        resourceManager_(resourceManager)
    {
        usableEntities_.push_front(0);

        Component* components[] = {
            (&GetComponent<ComponentTypes>())...
        };

        uint8_t ids[] = {
            (ComponentTypes::ID)...
        };

        for (int i = 0; i < sizeof...(ComponentTypes); i++) {
            componentIdMap_[ids[i]] = components[i];
        }
    }

    uint16_t CreateEntity() {
        uint16_t createdEntity = usableEntities_.front();
        usableEntities_.pop_front();
        if (usableEntities_.size() <= 0)
            usableEntities_.push_front(createdEntity + 1);

        entities_[createdEntity].alive_ = true;
        return createdEntity;
    }
    uint16_t CreateEntity(std::string entityName) {
        uint16_t createdEntity = CreateEntity();
        nlohmann::json entityData = resourceManager_.entities_[entityName];
        for (auto& componentData : entityData["components"]) {
            uint8_t id = componentData["id"].get<double>();
            entities_[createdEntity].componentMask_.set(id);
            componentIdMap_[id]->Load(componentData, createdEntity);
        }
        return createdEntity;
    }
    void DestroyEntity(uint16_t entityToDestroy) {
        entities_[entityToDestroy].alive_ = false;
        entities_[entityToDestroy].componentMask_ = 0;
        usableEntities_.push_front(entityToDestroy);
    }

    template<class T>
    auto RegisterComponent(uint16_t targetEntity) {
        assert(entities_[targetEntity].alive_);
        assert(!entities_[targetEntity].componentMask_.test(T::ID));

        entities_[targetEntity].componentMask_.set(T::ID);
        return std::get<T>(components_)[targetEntity];
    }

    template<class T>
    T& GetComponent() {
        return std::get<T>(components_);
    }

private:
    ResourceManager& resourceManager_;
    std::tuple<ComponentTypes...> components_;
    Component* componentIdMap_[MAX_COMPONENT_TYPES];
};

typedef EntityManagerTemp<
    TransformComponent,
    MovementComponent,
    GroundTraceComponent,
    VelocityComponent,
    ProjectileComponent,
    TerrainModComponent,
    WorldColliderComponent,
    SpreadActivatorComponent,
    SpreadDetectComponent,
    BubbleComponent,
    PickupComponent,
    HoldableComponent,
    KickerComponent,
    KickableComponent,
    IntervalSpawnComponent,
    StaticModelComponent
> EntityManager;
