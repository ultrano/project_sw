#ifndef SWTriangle_h__
#define SWTriangle_h__

#include "SWType.h"

class SWTriangle : public SWMemory
{
public:

	tushort index1;
	tushort index2;
	tushort index3;

	SWTriangle() : index1(0), index2(0), index3(0) {};
	SWTriangle( tushort i1, tushort i2, tushort i3) : index1(i1), index2(i2), index3(i3) {};
};

#endif // SWTriangle_h__