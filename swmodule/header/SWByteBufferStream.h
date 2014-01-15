#ifndef SWByteBufferStream_h__
#define SWByteBufferStream_h__

#include "SWByteBuffer.h"
#include "SWIOStream.h"

class SWByteBufferOutputStream : public SWOutputStream
{
	SW_RTTI( SWByteBufferOutputStream, SWInputStream );

	SWHardRef<SWByteBuffer> m_buffer;
	tuint m_cursor;

public:

	SWByteBufferOutputStream();
	SWByteBufferOutputStream( SWByteBuffer* buffer );
	
	void write(tbyte* b, tuint len);

	SWByteBuffer* getBuffer() const { return m_buffer(); };
};

class SWByteBufferInputStream : public SWInputStream
{
	SW_RTTI( SWByteBufferInputStream, SWInputStream );

	SWHardRef<SWByteBuffer> m_buffer;
	tuint m_cursor;

public:

	SWByteBufferInputStream();
	SWByteBufferInputStream( SWByteBuffer* buffer );
	SWByteBufferInputStream( SWInputStream* is );
	
	/* return : read byte count. return -1 if there is no more */
	int   read(tbyte* b, tuint len);
	tuint available();
	tuint skip( tuint len );
	
	SWByteBuffer* getBuffer() const { return m_buffer(); };
};


#endif //! SWByteBufferStream_h__