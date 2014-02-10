#ifndef TIndex3_h__
#define TIndex3_h__

#include "SWPrimaryType.h"

// TODO: change class category to Type (TIndex3 -> TIndex3 )
class TIndex3 : public SWMemory
{
public:

	tushort index1;
	tushort index2;
	tushort index3;

	TIndex3() : index1(0), index2(0), index3(0) {};
	TIndex3( tushort i1, tushort i2, tushort i3) : index1(i1), index2(i2), index3(i3) {};
};

#endif // TIndex3_h__