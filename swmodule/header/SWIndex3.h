#ifndef SWIndex3_h__
#define SWIndex3_h__

#include "SWType.h"

// TODO: change class category to Type (SWIndex3 -> TIndex3 )
class SWIndex3 : public SWMemory
{
public:

	tushort index1;
	tushort index2;
	tushort index3;

	SWIndex3() : index1(0), index2(0), index3(0) {};
	SWIndex3( tushort i1, tushort i2, tushort i3) : index1(i1), index2(i2), index3(i3) {};
};

#endif // SWIndex3_h__