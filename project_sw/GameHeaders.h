#ifndef GameHeaders_h__
#define GameHeaders_h__

#include "SWHeaders.h"
#include "Rider.h"
#include "GasCloud.h"
#include "Coin.h"
#include "BackGround.h"
#include "Obstacle.h"
#include "Character.h"
#include "CharacterState.h"
#include "VehicleItem.h"
#include "Bomb.h"

inline void registerGameAppFactories()
{
	SW_GC.registerFactory<Rider>();
	SW_GC.registerFactory<Character>();
	SW_GC.registerFactory<Runner>();
	SW_GC.registerFactory<Bird>();
	SW_GC.registerFactory<GasCloud>();
	SW_GC.registerFactory<Coin>();
	SW_GC.registerFactory<BackGround>();
	SW_GC.registerFactory<Obstacle>();
	SW_GC.registerFactory<VehicleItem>();
	SW_GC.registerFactory<Bomb>();
	SW_GC.registerFactory<Shrapnel>();
};

inline bool isButtonPushed()
{
	return SWInput.getKey( ' ' ) || SWInput.getTouchState() == SW_TouchPress || SWInput.getTouchState() == SW_TouchMove;
};

#endif // GameHeaders_h__