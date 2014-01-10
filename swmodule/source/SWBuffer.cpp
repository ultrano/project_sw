#include "SWBuffer.h"
#include "SWUtil.h"


SWBuffer::SWBuffer()
{
}

SWBuffer::SWBuffer( tuint size )
{
	resize( size );
}

SWBuffer::~SWBuffer()
{
	clear();
}

void SWBuffer::resize( tuint size )
{
	m_buf.resize( size );
}
void SWBuffer::clear()
{
	m_buf.clear();
	m_buf.resize( 0 );
}
void* SWBuffer::getPtr()
{
	return (void*)&m_buf[0];
}

tuint SWBuffer::size() const
{
	return m_buf.size();
}