#ifndef GameHeaders_h__
#define GameHeaders_h__

#include "Rider.h"
#include "GasCloud.h"
#include "Coin.h"
#include "BackGround.h"
#include "Obstacle.h"

void registerGameAppFactories()
{
	SW_GC.registerFactory<Rider>();
	SW_GC.registerFactory<GasCloud>();
	SW_GC.registerFactory<Coin>();
	SW_GC.registerFactory<BackGround>();
	SW_GC.registerFactory<Obstacle>();
}

#endif // GameHeaders_h__