#pragma once
#include "Entity.h"
#include "Player.h"
#include "TPillar.h"
#include "BumpRat.h"
#include "TeleportPod.h"
#include "RabbitPole.h"
#include "MultiHitPod.h"

#define EXPANDENTITIES \
	ENTITYEXP(Entity, entity, 0) \
	ENTITYEXP(Player, player, 1) \
	ENTITYEXP(BumpRat, bumprat, 2) \
	ENTITYEXP(TPillar, tpillar, 3) \
	ENTITYEXP(RabbitPole, rabbitpole, 4) \
	ENTITYEXP(TeleportPod, teleportpod, 5) \
	ENTITYEXP(MultiHitPod, multihitpod, 6)
