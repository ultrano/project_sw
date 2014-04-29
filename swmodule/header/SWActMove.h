#ifndef SWActMove_h__
#define SWActMove_h__

#include "SWAct.h"
class SWActMove : public SWAct
{
	SW_RTTI( SWActMove, SWAct );

public:

	SWActMove( float duration, const tvec3& from, const tvec3& to );
	~SWActMove();

protected:

	virtual bool isDone();
	virtual void onStart();
	virtual void onUpdate();

protected:

	float m_duration;
	float m_spendTime;
	tvec3 m_from;
	tvec3 m_to;
};

//////////////////////////////////////////////////////////////////////////
class SWActMoveTo : public SWActMove
{
	SW_RTTI( SWActMoveTo, SWActMove );

public:

	SWActMoveTo( float duration, const tvec3& to );
	~SWActMoveTo();

protected:

	virtual void onStart();

};

//////////////////////////////////////////////////////////////////////////
class SWActMoveFrom : public SWActMove
{
	SW_RTTI( SWActMoveFrom, SWActMove );

public:

	SWActMoveFrom( float duration, const tvec3& from );
	~SWActMoveFrom();

protected:

	virtual void onStart();

};

//////////////////////////////////////////////////////////////////////////
class SWActMoveBy : public SWActMove
{
	SW_RTTI( SWActMoveBy, SWActMove );

public:

	SWActMoveBy( float duration, const tvec3& by );
	~SWActMoveBy();

protected:

	virtual void onStart();

private:

	tvec3 m_by;

};
#endif // SWActMove_h__