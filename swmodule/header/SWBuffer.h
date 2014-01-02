#ifndef SWBuffer_h__
#define SWBuffer_h__

#include "SWRefCounter.h"
#include "SWType.h"

class SWBuffer : public SWRefCountable
{
	void* m_buf;
	size_t m_size;

public:
	
	SWBuffer();
	SWBuffer( tuint size );
	~SWBuffer();

	void allocMem( tuint size );
	void freeMem();
	void* getPtr();
	tuint size() const;

};
#endif // SWBuffer_h__