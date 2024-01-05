#pragma once
#include "EntityS.h"
#include "Player.h"
#include "BumpRat.h"

#define EXPANDENTITIES \
    ENTITYEXP(EntityS, entity) \
    ENTITYEXP(Player, player) \
    ENTITYEXP(BumpRat, bumpRat)
