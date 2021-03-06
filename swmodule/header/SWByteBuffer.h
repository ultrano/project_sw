#ifndef SWByteBuffer_h__
#define SWByteBuffer_h__

#include "SWRefCounter.h"
#include "SWType.h"

class SWByteBuffer : public SWRefCountable
{
	tarray< tbyte, SWAllocator<tbyte> > m_buf;

public:
	
	SWByteBuffer();
	SWByteBuffer( tuint size );
	~SWByteBuffer();

	void resize( tuint size );
	void clear();
	tbyte* getRaw();
	tuint size() const;

};
#endif // SWByteBuffer_h__