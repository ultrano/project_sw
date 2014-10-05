
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
	const T& clamp( const T& val, const T& edge1, const T& edge2 ) const
	{
		if ( edge2 > edge1 ) return (val < edge1)? edge1 : (val > edge2)? edge2 : val;
		else if ( edge2 < edge1 ) (val < edge2)? edge2 : (val > edge1)? edge1 : val;

		return edge1;
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
	float sqrt( float value ) const;

	float atan( float sinValue, float cosValue ) const;

	float radianToAngle( float radian ) const;
	float angleToRadian( float angle ) const;

	int   randomInt( int kMin, int kMax ) const;
	float randomFloat() const;
};

#endif //SWMath