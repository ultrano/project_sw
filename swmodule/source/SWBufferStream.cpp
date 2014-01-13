#include "SWBufferStream.h"
#include "SWMath.h"
#include <stdlib.h>


SWBufferOutputStream::SWBufferOutputStream()
	: m_buffer( new SWBuffer() )
	, m_cursor(0)
{
}
SWBufferOutputStream::SWBufferOutputStream( SWBuffer* buffer )
	: m_buffer( buffer )
	, m_cursor(0)
{
}

void SWBufferOutputStream::write(tbyte* b, tuint len)
{
	tuint capacity = m_cursor + len;
	if ( m_buffer()->size() < capacity ) m_buffer()->resize( capacity );

	tbyte* dst = &((tbyte*)m_buffer()->getPtr())[m_cursor];
	memcpy( dst, (void*)b, len );
	m_cursor = capacity;
}

////////////////////////////////////////////////////

SWBufferInputStream::SWBufferInputStream()
	: m_buffer( new SWBuffer() )
	, m_cursor(0)
{
}
SWBufferInputStream::SWBufferInputStream( SWBuffer* buffer )
	: m_buffer( buffer )
	, m_cursor(0)
{
}
SWBufferInputStream::SWBufferInputStream( SWInputStream* is )
	: m_buffer( new SWBuffer() )
	, m_cursor(0)
{
	m_buffer()->resize( is->available() );
	tbyte* dst = &((tbyte*)m_buffer()->getPtr())[0];
	is->read( dst, m_buffer()->size() );
}

int SWBufferInputStream::read(tbyte* b, tuint len)
{
	tuint remain = available() - m_cursor;
	if ( remain <= 0 ) return -1;

	tbyte* src = &((tbyte*)m_buffer()->getPtr())[m_cursor];
	len = SWMath.min( len, remain );

	memcpy( b, (void*)src[0], len );

	m_cursor = SWMath.min( m_cursor += len, available() );
	return len;
}

tuint SWBufferInputStream::available()
{
	return m_buffer()->size();
}

tuint SWBufferInputStream::skip( tuint len )
{
	tuint remain = available() - m_cursor;
	len = SWMath.min<tuint>( remain, len );

	m_cursor = SWMath.min( m_cursor += len, available() );

	return len;
}

void  SWBufferInputStream::reset()
{
	m_cursor = 0;
}