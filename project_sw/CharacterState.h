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
		None,
		Running,
		Boosting,
		Gliding,
		Jumping,
	};

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

	void updateState();
	void changeState();
	void setState( State state );
	State getState() const;

private:
	
	State m_state;
	State m_newState;
	bool  m_isChanged;

	bool m_activate;
	SWHardRef<SWRigidBody2D> m_body;
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

	virtual void onStart();
	virtual void onRemove();
	virtual void onUpdate();
	virtual void onFixedRateUpdate();
	virtual void onCollision( SWCollision2D* );

private:

	bool m_wasPressed;
	bool m_doFlapping;
};

#endif //! CharacterState_h__