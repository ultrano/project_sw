#ifndef TOBB_h__
#define TOBB_h__

#include "TVector2f.h"
#include "TVector3f.h"

class TOBB2D : SWMemory
{
public:

	TOBB2D()
		: radian(0)
	{
	}
	~TOBB2D() {};
	TVector2f center;
	TVector2f halfSize;
	float radian;

};

#endif // TOBB_h__