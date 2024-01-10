#pragma once
#include "Entity.h"
#include "Player.h"
#include "BumpRat.h"
#include "TPillar.h"
#include "RabbitPole.h"
#include "MagicSeed.h"

#define EXPANDENTITIES \
    ENTITYEXP(Entity, entity, 0) \
    ENTITYEXP(Player, player, 1) \
    ENTITYEXP(BumpRat, bumpRat, 2) \
    ENTITYEXP(TPillar, tpillar, 3) \
    ENTITYEXP(RabbitPole, rabbitPole, 4) \
    ENTITYEXP(MagicSeed, magicSeed, 5)
