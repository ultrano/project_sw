#include "SWRawBufferStream.h"

SWRawBufferOutputStream::SWRawBufferOutputStream()
	: m_buffer( NULL )
	, m_size(0)
	, m_cursor(0)
{
}
SWRawBufferOutputStream::SWRawBufferOutputStream( tbyte* buffer, tuint size )
	: m_buffer( buffer )
	, m_size( size )
	, m_cursor(0)
{
}

void SWRawBufferOutputStream::write(tbyte* b, tuint len)
{
	tuint capacity = m_cursor + len;
	if ( m_buffer()->size() < capacity ) m_buffer()->resize( capacity );

	tbyte* dst = &(m_buffer()->getRaw())[m_cursor];
	memcpy( dst, (void*)b, len );
	m_cursor = capacity;
}

////////////////////////////////////////////////////

SWRawBufferInputStream::SWRawBufferInputStream()
	: m_buffer( NULL )
	, m_size(0)
	, m_cursor(0)
{
}

SWRawBufferInputStream::SWRawBufferInputStream( tbyte* buffer, tuint size )
	: m_buffer( buffer )
	, m_size( size )
	, m_cursor(0)
{
}

int SWRawBufferInputStream::read(tbyte* b, tuint len)
{
	tuint remain = available() - m_cursor;
	if ( remain <= 0 ) return -1;

	tbyte* src = &(m_buffer()->getRaw())[m_cursor];
	len = SWMath.min( len, remain );

	memcpy( b, (void*)src, len );

	m_cursor = SWMath.min<tuint>( m_cursor += len, available() );
	return len;
}

int SWRawBufferInputStream::available()
{
	if ( m_buffer.isValid() == false ) return -1;
	return m_buffer()->size();
}

int SWRawBufferInputStream::skip( tuint len )
{
	tuint remain = available() - m_cursor;
	len = SWMath.min<tuint>( remain, len );

	m_cursor = SWMath.min<tuint>( m_cursor += len, available() );

	return len;
}
