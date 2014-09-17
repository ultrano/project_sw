#ifndef CharacterState_h__
#define CharacterState_h__

#include "SWHeaders.h"

class CharacterState : public SWBehavior
{
	SW_RTTI( CharacterState, SWBehavior );
};

class Runner : public CharacterState
{
	SW_RTTI( Runner, CharacterState );

public:

	Runner( factory_constructor );
	~Runner();

protected:

	virtual void onStart();
	virtual void onRemove();
	virtual void onUpdate();
	virtual void onFixedRateUpdate();
	virtual void onCollision( SWCollision2D* );
	
	void inactivate( SWActDelegate* del );

private:

	bool m_activate;
	tint m_boostDelayFrame;
	SWHardRef<SWRigidBody2D> m_body;
	SWHardRef<SWSpriteRenderer> m_renderer;
	SWHardRef<SWSpriteAtlas> m_imgAtlas;
	SWWeakRef<SWGameObject> m_jumpEffect;

	SWHardRef<SWAudioClip::Source> m_jumpSound;
	SWHardRef<SWAudioClip::Source> m_landSound;
	SWHardRef<SWAudioClip::Source> m_steamSound;
	SWHardRef<SWAudioClip::Source> m_shockSound;
	SWHardRef<SWAudioClip::Source> m_stepRSound[3];
	SWHardRef<SWAudioClip::Source> m_stepLSound[3];
	SWAudioClip::Source* m_stepSound;
	bool m_whichStep;

};

class Bird : public CharacterState
{
	SW_RTTI( Bird, CharacterState );
	
public:

	Bird( factory_constructor );
	~Bird();

protected:

	virtual void onStart();
	virtual void onRemove();
	virtual void onUpdate();
	virtual void onFixedRateUpdate();
	virtual void onCollision( SWCollision2D* );

private:

	bool m_wasPressed;
	bool m_doFlapping;

	SWHardRef<SWAudioClip::Source> m_flapSound[3];
	SWHardRef<SWAudioClip::Source> m_landSound;
	SWHardRef<SWAudioClip::Source> m_slideSound;
};

#endif //! CharacterState_h__