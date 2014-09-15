#ifndef SWActColor_h__
#define SWActColor_h__

#include "SWAct.h"

class SWActColor : public SWAct
{
	SW_RTTI( SWActColor, SWAct );
	
public:

	SWActColor( float duration, const tcolor& from, const tcolor& to );
	SWActColor( factory_constructor );
	~SWActColor();

protected:

	virtual bool isDone();
	virtual void onStart();
	virtual void onUpdate();
	
	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

protected:

	float m_duration;
	float m_spendTime;
	tcolor m_from;
	tcolor m_to;
};

//////////////////////////////////////////////////////////////////////////
class SWActColorTo : public SWActColor
{
	SW_RTTI( SWActColorTo, SWActColor );

public:

	SWActColorTo( float duration, const tcolor& to );
	SWActColorTo( factory_constructor );
	~SWActColorTo();

protected:

	virtual void onStart();

};
//////////////////////////////////////////////////////////////////////////
class SWActColorFrom : public SWActColor
{
	SW_RTTI( SWActColorFrom, SWActColor );

public:

	SWActColorFrom( float duration, const tcolor& from );
	SWActColorFrom( factory_constructor );
	~SWActColorFrom();

protected:

	virtual void onStart();

};

//////////////////////////////////////////////////////////////////////////
class SWActColorBy : public SWActColor
{
	SW_RTTI( SWActColorBy, SWActColor );

public:

	SWActColorBy( float duration, const tcolor& by );
	SWActColorBy( factory_constructor );
	~SWActColorBy();

protected:

	virtual void onStart();
	
	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

private:

	tcolor m_by;

};

#endif //SWActColor_h__