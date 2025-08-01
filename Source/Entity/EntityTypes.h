#pragma once
#include "Bomb.h"
#include "BumpRat.h"
#include "GroundSpawnProjectile.h"
#include "HeadSpitter.h"
#include "Mower.h"
#include "MultiHitPod.h"
#include "RabbitPole.h"
#include "RisePillar.h"
#include "Sleeper.h"
#include "SpeedBoost.h"
#include "TimedHitbox.h"
#include "TimerPod.h"
#include "TrailFish.h"
#include "Mole.h"
#include "Pig.h"
#include "TPillar.h"
#include "Pinata.h"
#include "Bird.h"
#include "SnakeHead.h"
#include "SnakeSegment.h"
#include "SnakePod.h"
#include "ItemContainer.h"
#include "PillarPad.h"
#include "AttackSegment.h"
#include "TriggerCrystal.h"
#include "Stomper.h"
#include "ItemToken.h"
#include "Ramp.h"
#include "HomingBossAttack.h"
#include "HomingBossPoint.h"
#include "HomingBoss.h"
#include "HomingBossProjectile.h"
#include "WaterGun.h"
#include "FlowerRamp.h"
#include "Water.h"
#include "Bumper.h"
#include "Popper.h"
#include "Segment.h"
#include "MeteorHole.h"
#include "Medal.h"
#include "JumpPad.h"
#include "Apple.h"
#include "Tree.h"
#include "Chase.h"
#include "HoleMarker.h"
#include "Negator.h"
#include "CometSpawner.h"
#include "SinglePod.h"
#include "Comet.h"
#include "PopInCrystalBreaker.h"
#include "SeedBalloon.h"
#include "TeleportPod.h"
#include "PopInCrystal.h"
#include "Peg.h"
#include "Player.h"
#include "Entity.h"
#include "HillBurrow.h"
#include "BigBreaker.h"

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
	ENTITYEXP(Ramp, ramp, 15) \
	ENTITYEXP(JumpPad, jumppad, 16) \
	ENTITYEXP(TimedHitbox, timedhitbox, 17) \
	ENTITYEXP(HeadSpitter, headspitter, 18) \
	ENTITYEXP(GroundSpawnProjectile, groundspawnprojectile, 19) \
	ENTITYEXP(Mole, mole, 20) \
	ENTITYEXP(RisePillar, risepillar, 21) \
	ENTITYEXP(SeedBalloon, seedballoon, 22) \
	ENTITYEXP(SpeedBoost, speedboost, 23) \
	ENTITYEXP(Apple, apple, 24) \
	ENTITYEXP(Tree, tree, 25) \
	ENTITYEXP(Pig, pig, 26) \
	ENTITYEXP(Pinata, pinata, 27) \
	ENTITYEXP(Bird, bird, 28) \
	ENTITYEXP(SnakeSegment, snakesegment, 29) \
	ENTITYEXP(SnakeHead, snakehead, 30) \
	ENTITYEXP(SnakePod, snakepod, 31) \
	ENTITYEXP(ItemContainer, itemcontainer, 32) \
	ENTITYEXP(ItemToken, itemtoken, 33) \
	ENTITYEXP(Segment, segment, 34) \
	ENTITYEXP(Negator, negator, 35) \
	ENTITYEXP(PillarPad, pillarpad, 36) \
	ENTITYEXP(Peg, peg, 37) \
	ENTITYEXP(AttackSegment, attacksegment, 38) \
	ENTITYEXP(Stomper, stomper, 39) \
	ENTITYEXP(TriggerCrystal, triggercrystal, 40) \
	ENTITYEXP(HomingBoss, homingboss, 41) \
	ENTITYEXP(HomingBossPoint, homingbosspoint, 42) \
	ENTITYEXP(HomingBossProjectile, homingbossprojectile, 43) \
	ENTITYEXP(HomingBossAttack, homingbossattack, 44) \
	ENTITYEXP(Chase, chase, 45) \
	ENTITYEXP(Bumper, bumper, 46) \
	ENTITYEXP(FlowerRamp, flowerramp, 47) \
	ENTITYEXP(Water, water, 48) \
	ENTITYEXP(WaterGun, watergun, 49) \
	ENTITYEXP(Popper, popper, 50) \
	ENTITYEXP(HoleMarker, holemarker, 51) \
	ENTITYEXP(MeteorHole, meteorhole, 52) \
	ENTITYEXP(Medal, medal, 53) \
	ENTITYEXP(PopInCrystalBreaker, popincrystalbreaker, 54) \
	ENTITYEXP(PopInCrystal, popincrystal, 55) \
	ENTITYEXP(HillBurrow, hillburrow, 56) \
	ENTITYEXP(BigBreaker, bigbreaker, 57)
