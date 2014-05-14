#ifndef SWActScale_h__
#define SWActScale_h__

#include "SWAct.h"

class SWActScale : public SWAct
{
	SW_RTTI( SWActScale, SWAct );

public:

	SWActScale( float duration, const tvec3& from, const tvec3& to );
	~SWActScale();

protected:

	virtual bool isDone();
	virtual void onStart();
	virtual void onUpdate();
	
	void serialize( SWObjectWriter* ow );
	void deserialize( SWObjectReader* or );

protected:

	float m_duration;
	float m_spendTime;
	tvec3 m_from;
	tvec3 m_to;

};
//////////////////////////////////////////////////////////////////////////
class SWActScaleTo : public SWActScale
{
	SW_RTTI( SWActScaleTo, SWActScale );

public:

	SWActScaleTo( float duration, const tvec3& to );
	~SWActScaleTo();

protected:

	virtual void onStart();

};

//////////////////////////////////////////////////////////////////////////
class SWActScaleFrom : public SWActScale
{
	SW_RTTI( SWActScaleFrom, SWActScale );

public:

	SWActScaleFrom( float duration, const tvec3& from );
	~SWActScaleFrom();

protected:

	virtual void onStart();

};

//////////////////////////////////////////////////////////////////////////
class SWActScaleBy : public SWActScale
{
	SW_RTTI( SWActScaleBy, SWActScale );

public:

	SWActScaleBy( float duration, const tvec3& by );
	~SWActScaleBy();

protected:

	virtual void onStart();
	
	void serialize( SWObjectWriter* ow );
	void deserialize( SWObjectReader* or );

private:

	tvec3 m_by;

};

#endif // SWActScale_h__