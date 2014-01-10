#include "SWBufferStream.h"

SWBufferInputStream::SWBufferInputStream()
	: m_cursor(0)
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
	const tuint bufsz = 512;
	tbyte buf[bufsz] = {0};
	tuint readsz = 0;
	tuint offset = 0;
	do
	{
		offset = m_buffer()->size();
		readsz = is->read( &buf[0], bufsz );
		if ( readsz <= 0 ) break;
		m_buffer()->resize( offset + readsz );
		tbyte* dst = &((tbyte*)m_buffer()->getPtr())[offset];
		memcpy( dst, (void*)buf[0], readsz );
	} while ( true );
}

int SWBufferInputStream::read(tbyte* b, tuint len)
{
	return -1;
}