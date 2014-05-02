
#ifndef __SWMath_h
#define __SWMath_h

#include "SWType.h"

#define SWMath (__SWMath::getInstance())

class __SWMath
{
private:
	__SWMath();
public:
	static const __SWMath& getInstance();

	const float pi;

	template<typename T>
	T abs( const T& v ) const { return (v<0)? -v:v; };
	
	template<typename T>
	const T& max( const T& a, const T& b) const { return (a<b)? b:a; };

	template<typename T>
	const T& min( const T& a, const T& b) const { return (a<b)? a:b; };

	template<typename T>
	const T& clamp( const T& kMin, const T& kMax, const T& val ) const
	{
		return ( val < kMin )? kMin : ( val > kMax )? kMax : val;
	};

	inline float repeat( float t, float length ) const
	{
		int times = ((int)(t/length));
		return (t - ( times*length ));
	};

	inline float pingPong( float t, float length ) const
	{
		t = repeat(  t, length*2 );
		return ( length - abs( t - length ) );
	};

	float sin( float radian ) const;
	float cos( float radian ) const;
	float tan( float radian ) const;

	float atan( float sinValue, float cosValue ) const;

	float radianToAngle( float radian ) const;
	float angleToRadian( float angle ) const;

	int   randomInt( int kMin, int kMax ) const;
};

#endif //SWMath