#ifndef SWActAnimate_h__
#define SWActAnimate_h__

#include "SWAct.h"

class SWSprite;
class SWSpriteSequence;
class SWActAnimate : public SWAct
{
	SW_RTTI( SWActAnimate, SWAct );

public:

	SWActAnimate( float speed, SWSpriteSequence* sequence );
	SWActAnimate( factory_constructor );
	~SWActAnimate();

protected:

	virtual bool isDone();
	virtual void onStart();
	virtual void onUpdate();
	
	void serialize( SWObjectWriter* ow );
	void deserialize( SWObjectReader* or );

private:

	void changeSpriteWithAt( tuint index );

private:

	SWHardRef<SWSpriteSequence> m_sequence;
	float m_speed;
	tuint m_lastIndex;
	float m_spendTime;
	float m_totalTime;
};

#endif // SWActAnimate_h__