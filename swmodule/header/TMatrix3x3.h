#ifndef TMatrix3x3_h__
#define TMatrix3x3_h__

#include "TVector2f.h"
#include "TVector3f.h"

class TMatrix3x3 : public SWMemory
{
public:
	union
	{
		struct 
		{
			float m11,m12,m13;
			float m21,m22,m23;
			float m31,m32,m33;
		};
		float m[3][3];
	};

	void set( const TVector2f& scale, float rotate, const TVector2f& move );
};

const TMatrix3x3 operator * ( const TMatrix3x3& a, const TMatrix3x3& b );
const TVector2f  operator * ( const TVector2f& v, const TMatrix3x3& m );
const TVector3f  operator * ( const TVector3f& v, const TMatrix3x3& m );

#endif // TMatrix3x3_h__