#ifndef Character_h__
#define Character_h__

#include "SWHeaders.h"

class Character : public SWBehavior
{
	SW_RTTI( Character, SWBehavior );

public:
	
	Character( factory_constructor );
	~Character();

	void setScore( tuint score ) { m_score = score; };
	tuint getScore() const { return m_score; };

protected:

	virtual void onAwake();
	virtual void onStart();
	virtual void onUpdate();
	virtual void onFixedRateUpdate();
	virtual void onCollision( SWCollision2D* );

private:

	tuint m_score;
	SWWeakRef<SWFontRenderer> m_meterScore;
	SWWeakRef<SWFontRenderer> m_coinScore;
};

#endif //! Character_h__