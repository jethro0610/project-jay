#pragma once
#include "Bomb.h"
#include "BumpRat.h"
#include "Comet.h"
#include "CometSpawner.h"
#include "GroundSpawnProjectile.h"
#include "HeadSpitter.h"
#include "ItemBox.h"
#include "JumpPad.h"
#include "MeteorBox.h"
#include "Mower.h"
#include "MultiHitPod.h"
#include "Player.h"
#include "RabbitPole.h"
#include "RisePillar.h"
#include "SinglePod.h"
#include "Sleeper.h"
#include "SpeedBoost.h"
#include "TPillar.h"
#include "TeleportPod.h"
#include "TimedHitbox.h"
#include "TimerPod.h"
#include "TrailFish.h"
#include "Mole.h"
#include "Apple.h"
#include "Entity.h"
#include "Tree.h"

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
	ENTITYEXP(Sleeper, sleeper, 12) \
	ENTITYEXP(TimerPod, timerpod, 13) \
	ENTITYEXP(SinglePod, singlepod, 14) \
	ENTITYEXP(ItemBox, itembox, 15) \
	ENTITYEXP(JumpPad, jumppad, 16) \
	ENTITYEXP(TimedHitbox, timedhitbox, 17) \
	ENTITYEXP(HeadSpitter, headspitter, 18) \
	ENTITYEXP(GroundSpawnProjectile, groundspawnprojectile, 19) \
	ENTITYEXP(Mole, mole, 20) \
	ENTITYEXP(RisePillar, risepillar, 21) \
	ENTITYEXP(MeteorBox, meteorbox, 22) \
	ENTITYEXP(SpeedBoost, speedboost, 23) \
	ENTITYEXP(Apple, apple, 24) \
	ENTITYEXP(Tree, tree, 25)
