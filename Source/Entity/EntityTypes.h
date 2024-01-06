#pragma once
#include "Entity.h"
#include "Player.h"
#include "BumpRat.h"

#define EXPANDENTITIES \
    ENTITYEXP(Entity, entity) \
    ENTITYEXP(Player, player) \
    ENTITYEXP(BumpRat, bumpRat)
