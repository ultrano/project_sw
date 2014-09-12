#ifndef SWActRotate_h__
#define SWActRotate_h__

#include "SWAct.h"

class SWActRotate : public SWAct
{
	SW_RTTI( SWActRotate, SWAct );

public:

	SWActRotate( float duration, const tvec3& from, const tvec3& to );
	SWActRotate( factory_constructor );
	~SWActRotate();

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
class SWActRotateTo : public SWActRotate
{
	SW_RTTI( SWActRotateTo, SWActRotate );

public:

	SWActRotateTo( float duration, const tvec3& to );
	SWActRotateTo( factory_constructor );
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
	SWActRotateFrom( factory_constructor );
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
	SWActRotateBy( factory_constructor );
	~SWActRotateBy();

protected:

	virtual void onStart();
	
	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

private:

	tvec3 m_by;

};

#endif // SWActRotate_h__