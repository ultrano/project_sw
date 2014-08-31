#ifndef ScoreBoard_h__
#define ScoreBoard_h__

#include "SWHeaders.h"

class Rider;

class ScoreBoard : public SWBehavior
{
	SW_RTTI( ScoreBoard, SWBehavior );

public:

	ScoreBoard();
	ScoreBoard( factory_constructor );
	~ScoreBoard();

protected:

	virtual void onAwake();
	virtual void onFixedRateUpdate();

private:

	SWWeakRef<Rider> m_rider;
};

#endif // ScoreBoard_h__