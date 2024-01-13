#pragma once
#include "Player.h"
#include "TPillar.h"
#include "BumpRat.h"
#include "RabbitPole.h"
#include "TeleportPod.h"
#include "Entity.h"
#include "MultiHitPod.h"
#include "Bomb.h"
#include "TrailFish.h"

#define EXPANDENTITIES \
	ENTITYEXP(Entity, entity, 0) \
	ENTITYEXP(Player, player, 1) \
	ENTITYEXP(BumpRat, bumprat, 2) \
	ENTITYEXP(TPillar, tpillar, 3) \
	ENTITYEXP(RabbitPole, rabbitpole, 4) \
	ENTITYEXP(TeleportPod, teleportpod, 5) \
	ENTITYEXP(MultiHitPod, multihitpod, 6) \
	ENTITYEXP(TrailFish, trailfish, 7) \
	ENTITYEXP(Bomb, bomb, 8)
