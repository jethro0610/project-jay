#pragma once
#include "Entity.h"
#include "Player.h"
#include "BumpRat.h"
#include "TPillar.h"
#include "RabbitPole.h"
#include "MagicSeed.h"

#define EXPANDENTITIES \
    ENTITYEXP(Entity, entity) \
    ENTITYEXP(Player, player) \
    ENTITYEXP(BumpRat, bumpRat) \
    ENTITYEXP(TPillar, tpillar) \
    ENTITYEXP(RabbitPole, rabbitPole) \
    ENTITYEXP(MagicSeed, magicSeed)
