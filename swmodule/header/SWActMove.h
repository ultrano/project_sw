#ifndef SWActMove_h__
#define SWActMove_h__

#include "SWAct.h"

class SWActMove : public SWAct
{
	SW_RTTI( SWActMove, SWAct );

public:

	SWActMove( float duration, const tvec3& from, const tvec3& to );
	SWActMove( factory_constructor );
	~SWActMove();

protected:

	virtual bool isDone();
	virtual void onStart();
	virtual void onUpdate();
	
	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

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
	SWActMoveTo( factory_constructor );
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
	SWActMoveFrom( factory_constructor );
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
	SWActMoveBy( factory_constructor );
	~SWActMoveBy();

protected:

	virtual void onStart();
	
	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

private:

	tvec3 m_by;

};
#endif // SWActMove_h__