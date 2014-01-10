#ifndef SWBufferStream_h__
#define SWBufferStream_h__

#include "SWBuffer.h"
#include "SWIOStream.h"

class SWBufferInputStream : public SWInputStream
{
	SW_RTTI( SWBufferInputStream, SWInputStream );

	SWHardRef<SWBuffer> m_buffer;
	tuint m_cursor;

	SWBufferInputStream();
	SWBufferInputStream( SWBuffer* buffer );
	SWBufferInputStream( SWInputStream* is );
	
	/* return : read byte count. return -1 if there is no more */
	virtual int read(tbyte* b, tuint len);
};


#endif //! SWBufferStream_h__