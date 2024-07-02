#pragma once
#include "Bomb.h"
#include "BumpRat.h"
#include "MultiHitPod.h"
#include "RabbitPole.h"
#include "TPillar.h"
#include "TrailFish.h"
#include "Comet.h"
#include "CometSpawner.h"
#include "Entity.h"
#include "TeleportPod.h"
#include "Player.h"
#include "Detacher.h"
#include "Mower.h"

#define EXPANDENTITIES \
	ENTITYEXP(Entity, entity, 0) \
	ENTITYEXP(Player, player, 1) \
	ENTITYEXP(BumpRat, bumprat, 2) \
	ENTITYEXP(TPillar, tpillar, 3) \
	ENTITYEXP(RabbitPole, rabbitpole, 4) \
	ENTITYEXP(TeleportPod, teleportpod, 5) \
	ENTITYEXP(MultiHitPod, multihitpod, 6) \
	ENTITYEXP(TrailFish, trailfish, 7) \
	ENTITYEXP(Bomb, bomb, 8) \
	ENTITYEXP(Comet, comet, 9) \
	ENTITYEXP(CometSpawner, cometspawner, 10) \
	ENTITYEXP(Mower, mower, 11) \
	ENTITYEXP(Detacher, detacher, 12)
