#ifndef Bomb_h__
#define Bomb_h__

#include "SWHeaders.h"

class Bomb : public SWBehavior
{
	SW_RTTI( Bomb, SWBehavior );

public:

	Bomb( factory_constructor );
	~Bomb();
	
	void reset( const tvec3& pos );

protected:
	virtual void onAwake();
	virtual void onStart();
	virtual void onUpdate();
	virtual void onFixedRateUpdate();
	virtual void onCollision( SWCollision2D* );
	void onEndAction();
private:
	SWHardRef<SWAudioClip::Source> m_sound;
};

class Shrapnel : public SWBehavior
{
	SW_RTTI( Shrapnel, SWBehavior );

public:

	Shrapnel( factory_constructor );
	~Shrapnel();

	void reset();

protected:
	virtual void onAwake();
	virtual void onStart();
	virtual void onFixedRateUpdate();
	void onEndAction(SWActDelegate* act);

private:
	SWHardRef<SWSpriteAtlas> m_atlas;
	tint m_delay;
};

#endif // Bomb_h__