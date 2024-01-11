#pragma once
#include "Entity.h"
#include "Player.h"
#include "TPillar.h"
#include "RabbitPole.h"
#include "BumpRat.h"
#include "MagicSeed.h"

#define EXPANDENTITIES \
	ENTITYEXP(Entity, entity, 0) \
	ENTITYEXP(Player, player, 1) \
	ENTITYEXP(BumpRat, bumprat, 2) \
	ENTITYEXP(TPillar, tpillar, 3) \
	ENTITYEXP(RabbitPole, rabbitpole, 4) \
	ENTITYEXP(MagicSeed, magicseed, 5)
