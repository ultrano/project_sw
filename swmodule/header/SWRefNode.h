#ifndef SWRefNode_h__
#define SWRefNode_h__

#include "SWRefCounter.h"

class SWRefNode : public SWRefCountable
{
public:
	SWHardRef<SWRefNode> next;
	SWWeakRef<SWRefNode> prev;
	SWHardRef<SWRefCountable> ref;
	SWRefNode();
	~SWRefNode();
};

#endif // SWRefNode_h__