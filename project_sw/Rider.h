#ifndef Rider_h__
#define Rider_h__

#include "SWHeaders.h"

class Rider : public SWBehavior
{
	SW_RTTI( Rider, SWBehavior );

public:

	enum State
	{
		Running,
		Flying,
		Gliding,
		Landing,
	};

public:
	
	Rider();
	Rider( factory_constructor );
	~Rider();

	void setScore( tuint score ) { m_score = score; };
	tuint getScore() const { return m_score; };

protected:

	virtual void onAwake();
	virtual void onStart();
	virtual void onRemove();
	virtual void onUpdate();
	virtual void onFixedRateUpdate();
	virtual void onCollision( SWCollision2D* );

private:

	State m_state;
	SWHardRef<SWSpriteRenderer> m_renderer;
	SWHardRef<SWSpriteAtlas> m_imgAtlas;
	SWWeakRef<SWFontRenderer> m_meterScore;
	SWWeakRef<SWFontRenderer> m_coinScore;
	tuint m_score;

};

#endif //! Rider_h__