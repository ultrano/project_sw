#ifndef SWRawBufferStream_h__
#define SWRawBufferStream_h__

#include "SWIOStream.h"

class SWRawBufferOutputStream : public SWOutputStream
{
	SW_RTTI( SWRawBufferOutputStream, SWInputStream );

	tbyte* m_buffer;
	tuint  m_size;
	tuint  m_cursor;

public:

	SWRawBufferOutputStream();
	SWRawBufferOutputStream( tbyte* buffer, tuint size );
	
	void write(tbyte* b, tuint len);

};

class SWRawBufferInputStream : public SWInputStream
{
	SW_RTTI( SWRawBufferInputStream, SWInputStream );
	
	tbyte* m_buffer;
	tuint  m_size;
	tuint m_cursor;

public:

	SWByteBufferInputStream();
	SWByteBufferInputStream( tbyte* buffer, tuint size );
	
	/* return : read byte count. return -1 if there is no more */
	int read(tbyte* b, tuint len);
	int skip( tuint len );
	int available();
	
};

#endif //! SWRawBufferStream_h__