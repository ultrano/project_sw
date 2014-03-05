#ifndef SWAnimationLine_h__
#define SWAnimationLine_h__

#include "SWRefCounter.h"
#include "SWType.h"

class SWAnimationLine : public SWRefCountable
{
	SW_REFERENCE( SWAnimationLine );
public:
	
	struct KeyFrame : SWMemory
	{
		float time;
		float value;

		KeyFrame() : time(0), value(0) {};
		KeyFrame( float _time, float _value ) : time( _time ), value( _value ) {};
	};

private:

	typedef tarray<int> AlignedIndices;

	tarray<KeyFrame> m_keyFrames;
	AlignedIndices m_aligned;

public:

	SWAnimationLine();
	~SWAnimationLine();

	int  addKey( float time, float value );
	void removeKey( int index );
	float evaluate( float time );
	float beginTime();
	float endTime();

	static SWAnimationLine::Ref Linear( float beginTime, float endTime, float beginVal, float endVal );
};

#endif // SWAnimationLine_h__