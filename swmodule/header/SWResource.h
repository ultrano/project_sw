#ifndef SWAsset_h__
#define SWAsset_h__

#include "SWRefCounter.h"
#include "SWRtti.h"
#include "SWType.h"

class SWResource : public SWRefCountable
{
	SW_RTTI_ROOT( SWResource );
};

#endif // SWAsset_h__