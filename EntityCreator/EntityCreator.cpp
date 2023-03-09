#define ENTITYCREATOR
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "../Source/Game/Components/Components.h"

using namespace std;

ComponentDesc descriptions[32];
uint8_t componentCount = 0;

template <class T>
void RegisterDescription() {
    descriptions[T::ID] = T::description;
    componentCount++;
}

int main() {
    RegisterDescription<BubbleComponent>();
    RegisterDescription<GroundTraceComponent>();
    RegisterDescription<HoldableComponent>();
    RegisterDescription<IntervalSpawnComponent>();
    RegisterDescription<KickableComponent>();
    RegisterDescription<KickerComponent>();
    RegisterDescription<MovementComponent>();
    RegisterDescription<PickupComponent>();
    RegisterDescription<ProjectileComponent>();
    RegisterDescription<SpreadActivatorComponent>();
    RegisterDescription<SpreadDetectComponent>();
    RegisterDescription<StaticModelComponent>();
    RegisterDescription<TerrainModComponent>();
    RegisterDescription<TransformComponent>();
    RegisterDescription<VelocityComponent>();
    RegisterDescription<WorldColliderComponent>();
    bool enabledComponents[32];
    fill_n(enabledComponents, 32, false);

    cout << left;
    cout << "\n=======PROJECT JAY ENTITY CREATOR=======\n";
    cout << "Components\n";
    for (int i = 0; i < componentCount; i++) {
        cout << setw(4) << i << '[' << '-' << ']' << descriptions[i].name << '\n';
    }
    int16_t currentComponent = -1;
    cout << "Select: ";
    cin >> currentComponent;
    enabledComponents[currentComponent] = true;

    ComponentDesc& currentDesc = descriptions[currentComponent];
    cout << "\n=======" << currentDesc.name << "=======\n";
    for (int i = 0; i < currentDesc.propertyCount; i++) {
        cout << setw(4) << i << setw(20) << currentDesc.properties[i].name << setw(8) << currentDesc.properties[i].type << '\n';
    }
}
