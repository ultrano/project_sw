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
	
	enum State
	{
		Running,
		Flying,
		Gliding,
	};

public:

	Runner( factory_constructor );
	~Runner();

protected:

	virtual void onAwake();
	virtual void onRemove();
	virtual void onUpdate();
	virtual void onFixedRateUpdate();
	virtual void onCollision( SWCollision2D* );
	
	void inactivate( SWActDelegate* del );

private:
	
	State m_state;
	SWHardRef<SWSpriteRenderer> m_renderer;
	SWHardRef<SWSpriteAtlas> m_imgAtlas;
	SWWeakRef<SWGameObject> m_jumpEffect;

};

class Bird : public CharacterState
{
	SW_RTTI( Bird, CharacterState );
	
public:

	Bird( factory_constructor );
	~Bird();

protected:

	virtual void onAwake();
	virtual void onRemove();
	virtual void onUpdate();
	virtual void onFixedRateUpdate();
	virtual void onCollision( SWCollision2D* );

private:

	bool m_wasPressed;
	bool m_doFlapping;
};

#endif //! CharacterState_h__