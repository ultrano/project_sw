#ifndef SWActRotate_h__
#define SWActRotate_h__

#include "SWAct.h"

class SWActRotate : public SWAct
{
	SW_RTTI( SWActRotate, SWAct );

public:

	SWActRotate( float duration, const tvec3& from, const tvec3& to );
	~SWActRotate();

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
class SWActRotateTo : public SWActRotate
{
	SW_RTTI( SWActRotateTo, SWActRotate );

public:

	SWActRotateTo( float duration, const tvec3& to );
	~SWActRotateTo();

protected:

	virtual void onStart();

};

//////////////////////////////////////////////////////////////////////////
class SWActRotateFrom : public SWActRotate
{
	SW_RTTI( SWActRotateFrom, SWActRotate );

public:

	SWActRotateFrom( float duration, const tvec3& from );
	~SWActRotateFrom();

protected:

	virtual void onStart();

};

//////////////////////////////////////////////////////////////////////////
class SWActRotateBy : public SWActRotate
{
	SW_RTTI( SWActRotateBy, SWActRotate );

public:

	SWActRotateBy( float duration, const tvec3& by );
	~SWActRotateBy();

protected:

	virtual void onStart();

private:

	tvec3 m_by;

};

#endif // SWActRotate_h__