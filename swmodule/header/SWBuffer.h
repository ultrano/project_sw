#ifndef SWBuffer_h__
#define SWBuffer_h__

#include "SWRefCounter.h"
#include "SWType.h"

class SWBuffer : public SWRefCountable
{
	tarray<tbyte> m_buf;

public:
	
	SWBuffer();
	SWBuffer( tuint size );
	~SWBuffer();

	void resize( tuint size );
	void clear();
	void* getPtr();
	tuint size() const;

};
#endif // SWBuffer_h__