#include "SWMath.h"
#include <math.h>

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