#include "SWRawBufferStream.h"
#include "SWMath.h"

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
	if ( m_buffer == NULL ) return;
	if ( m_size == 0 ) return;

	if ( b == NULL ) return;
	if ( len == 0 ) return;

	tuint remain = m_size - m_cursor;
	len = SWMath.min( len, remain );
	if ( len == 0 ) return;

	memcpy( m_buffer + m_cursor, (void*)b, len );
	m_cursor += len;
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
	if ( m_buffer == NULL ) return -1;
	if ( m_size == 0 ) return -1;

	if ( b == NULL ) return 0;
	if ( len == 0 ) return 0;

	tuint remain = available() - m_cursor;
	if ( remain <= 0 ) return -1;

	len = SWMath.min( len, remain );

	memcpy( b, m_buffer + m_cursor, len );

	m_cursor += len;
	return len;
}

int SWRawBufferInputStream::available()
{
	if ( m_buffer == NULL ) return -1;
	return m_size;
}

int SWRawBufferInputStream::skip( tuint len )
{
	tuint remain = available() - m_cursor;
	len = SWMath.min( remain, len );
	m_cursor += len;

	return len;
}
