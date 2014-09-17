#ifndef Bomb_h__
#define Bomb_h__

#include "SWHeaders.h"

class Bomb : public SWBehavior
{
	SW_RTTI( Bomb, SWBehavior );

public:

	Bomb( factory_constructor );
	~Bomb();
	
protected:
	virtual void onStart();
	virtual void onUpdate();
	virtual void onFixedRateUpdate();
	virtual void onCollision( SWCollision2D* );
private:
	SWHardRef<SWAudioClip::Source> m_sound;
};

class Shrapnel : public SWBehavior
{
	SW_RTTI( Shrapnel, SWBehavior );

public:

	Shrapnel( factory_constructor );
	~Shrapnel();

protected:
	virtual void onStart();
	virtual void onFixedRateUpdate();

private:
	SWHardRef<SWSpriteAtlas> m_atlas;
	tuint m_delay;
};

#endif // Bomb_h__