#pragma once
#include <bitset>
#define MAX_ENTITIES 100
#define MAX_COMPONENT_TYPES 32
#define PLAYER_ENTITY 0
#define MAX_COMPONENT_TYPES 32

typedef std::bitset<MAX_COMPONENT_TYPES> ComponentMask;

struct Entity {
    bool alive;
    ComponentMask componentMask;
};