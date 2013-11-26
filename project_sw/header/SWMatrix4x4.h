#ifndef SWMatrix4x4_h__
#define SWMatrix4x4_h__

#include "SWQuaternion.h"

class SWMatrix4x4
{
public:

	const static SWMatrix4x4 unit;

public:
	union
	{
		struct 
		{
			float m11,m12,m13,m14;
			float m21,m22,m23,m24;
			float m31,m32,m33,m34;
			float m41,m42,m43,m44;
		};
		float m[4][4];
	};

	SWMatrix4x4();

	SWMatrix4x4& operator = ( const SWMatrix4x4& mat );
	SWMatrix4x4& operator *= ( float f );
	SWMatrix4x4& operator *= ( const SWMatrix4x4& mat );

	void		identity();
	float		determinant() const;
	float		minorDet(unsigned char row, unsigned char col) const;
	void		inverse(SWMatrix4x4& m) const;
	void		adjoint(SWMatrix4x4& m) const;
	void		transpose(SWMatrix4x4& t) const;
	
	SWMatrix4x4& transform(const SWVector3f& scale, const SWQuaternion& rotate, const SWVector3f& translate );
	SWMatrix4x4& ortho( float left, float right, float top, float bottom, float near, float far );
	
	SWQuaternion	row(unsigned char r) const;
	SWQuaternion	col(unsigned char c) const;


};

const SWMatrix4x4 operator * ( const SWMatrix4x4& a, const SWMatrix4x4& b );
const SWVector3f  operator * ( const SWVector3f& v, const SWMatrix4x4& m );


#endif // SWMatrix4x4_h__