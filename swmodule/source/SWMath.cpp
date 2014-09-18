#include "SWMath.h"
#include <math.h>
#include <stdlib.h>

const __SWMath& __SWMath::getInstance()
{
	static __SWMath instance;
	return instance;
}

__SWMath::__SWMath()
	: pi( 3.14f )
{
}

float __SWMath::sin( float radian ) const
{
	return sinf( radian );
}

float __SWMath::cos( float radian ) const
{
	return cosf( radian );
}

float __SWMath::tan( float radian ) const
{
	return tanf( radian );
}

float __SWMath::atan( float sinValue, float cosValue ) const
{
	return atan2f( sinValue, cosValue );
}

float __SWMath::radianToAngle( float radian ) const
{
	return (radian*(180.0f/pi));
}

float __SWMath::angleToRadian( float angle ) const
{
	return (angle*(pi/180.0f));
}


int __SWMath::randomInt( int kMin, int kMax ) const
{
	if ( kMin > kMax )
	{
		int temp = kMin;
		kMin = kMax;
		kMax = temp;
	}
	return kMin + rand() % ( 1 + kMax - kMin );
}

float __SWMath::randomFloat() const
{
	int max = 0xffff;
	int ret = randomInt( 0, max );
	return (float)ret/(float)max;
}
