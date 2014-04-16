#include "SWByteBuffer.h"
#include "SWUtil.h"


SWByteBuffer::SWByteBuffer()
{
}

SWByteBuffer::SWByteBuffer( tuint size )
{
	resize( size );
}

SWByteBuffer::~SWByteBuffer()
{
	clear();
}

void SWByteBuffer::resize( tuint size )
{
	m_buf.resize( size );
}
void SWByteBuffer::clear()
{
	m_buf.clear();
	m_buf.resize( 0 );
}
tbyte* SWByteBuffer::getRaw()
{
	return &m_buf[0];
}

tuint SWByteBuffer::size() const
{
	return m_buf.size();
}