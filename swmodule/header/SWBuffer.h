#ifndef SWBuffer_h__
#define SWBuffer_h__

#include "SWRefCounter.h"

class SWBuffer : public SWRefCountable
{
	void* m_buf;
	size_t m_size;

public:
	
	SWBuffer();
	~SWBuffer();

	void allocMem( size_t size );
	void freeMem();
	void* getPtr();

};
#endif // SWBuffer_h__