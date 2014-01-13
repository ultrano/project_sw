#ifndef SWBufferStream_h__
#define SWBufferStream_h__

#include "SWBuffer.h"
#include "SWIOStream.h"

class SWBufferOutputStream : public SWOutputStream
{
	SW_RTTI( SWBufferOutputStream, SWInputStream );

	SWHardRef<SWBuffer> m_buffer;
	tuint m_cursor;

public:

	SWBufferOutputStream();
	SWBufferOutputStream( SWBuffer* buffer );
	
	void write(tbyte* b, tuint len);

	SWBuffer* getBuffer() const { return m_buffer(); };
};

class SWBufferInputStream : public SWInputStream
{
	SW_RTTI( SWBufferInputStream, SWInputStream );

	SWHardRef<SWBuffer> m_buffer;
	tuint m_cursor;

public:

	SWBufferInputStream();
	SWBufferInputStream( SWBuffer* buffer );
	SWBufferInputStream( SWInputStream* is );
	
	/* return : read byte count. return -1 if there is no more */
	int   read(tbyte* b, tuint len);
	tuint available();
	tuint skip( tuint len );
	void  reset();
	
	SWBuffer* getBuffer() const { return m_buffer(); };
};


#endif //! SWBufferStream_h__