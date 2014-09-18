#ifndef Character_h__
#define Character_h__

#include "SWHeaders.h"

class Character : public SWBehavior
{
	SW_RTTI( Character, SWBehavior );

public:

	enum
	{
		None,

		Landing,
		OnGround,
		TakeOff,

		AirBorne,

		GotRoof,
		UnderRoof,
		LeaveRoof,
	};

public:
	
	Character( factory_constructor );
	~Character();

	void setScore( tuint score ) { m_score = score; };
	tuint getScore() const { return m_score; };

	tuint getState() const { return m_state; }
	bool isState( tuint state ) const { return ( m_state == state ); };

	SWAudioClip* getAudioClip( const tstring& filePath );

protected:

	virtual void onAwake();
	virtual void onStart();
	virtual void onUpdate();
	virtual void onFixedRateUpdate();
	virtual void onCollision( SWCollision2D* );

private:

	void updateCondition();

private:

	typedef ttable<tstring,SWHardRef<SWAudioClip>> AudioTable;

	tuint m_score;
	tuint m_state;
	SWWeakRef<SWFontRenderer> m_meterScore;
	SWWeakRef<SWFontRenderer> m_coinScore;
	SWHardRef<SWAudioClip::Source> m_coinSound[3];
	AudioTable m_audioTable;
};

#endif //! Character_h__