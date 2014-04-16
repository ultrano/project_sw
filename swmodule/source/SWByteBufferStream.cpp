#include "SWByteBufferStream.h"
#include "SWMath.h"
#include <stdlib.h>


SWByteBufferOutputStream::SWByteBufferOutputStream()
	: m_buffer( new SWByteBuffer() )
	, m_cursor(0)
{
}
SWByteBufferOutputStream::SWByteBufferOutputStream( SWByteBuffer* buffer )
	: m_buffer( buffer )
	, m_cursor(0)
{
}

void SWByteBufferOutputStream::write(tbyte* b, tuint len)
{
	tuint capacity = m_cursor + len;
	if ( m_buffer()->size() < capacity ) m_buffer()->resize( capacity );

	tbyte* dst = &(m_buffer()->getRaw())[m_cursor];
	memcpy( dst, (void*)b, len );
	m_cursor = capacity;
}

////////////////////////////////////////////////////

SWByteBufferInputStream::SWByteBufferInputStream()
	: m_buffer( new SWByteBuffer() )
	, m_cursor(0)
{
}
SWByteBufferInputStream::SWByteBufferInputStream( SWByteBuffer* buffer )
	: m_buffer( buffer )
	, m_cursor(0)
{
}
SWByteBufferInputStream::SWByteBufferInputStream( SWInputStream* is )
	: m_buffer( new SWByteBuffer() )
	, m_cursor(0)
{
	m_buffer()->resize( is->available() );
	is->read( m_buffer()->getRaw(), m_buffer()->size() );
}

int SWByteBufferInputStream::read(tbyte* b, tuint len)
{
	tuint remain = available() - m_cursor;
	if ( remain <= 0 ) return -1;

	tbyte* src = &(m_buffer()->getRaw())[m_cursor];
	len = SWMath.min( len, remain );

	memcpy( b, (void*)src[0], len );

	m_cursor = SWMath.min<tuint>( m_cursor += len, available() );
	return len;
}

int SWByteBufferInputStream::available()
{
	if ( m_buffer.isValid() == false ) return -1;
	return m_buffer()->size();
}

int SWByteBufferInputStream::skip( tuint len )
{
	tuint remain = available() - m_cursor;
	len = SWMath.min<tuint>( remain, len );

	m_cursor = SWMath.min<tuint>( m_cursor += len, available() );

	return len;
}
