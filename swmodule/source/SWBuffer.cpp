#include "SWBuffer.h"
#include "SWUtil.h"


SWBuffer::SWBuffer()
	: m_buf( NULL )
	, m_size( 0 )
{
}
SWBuffer::~SWBuffer()
{
	freeMem();
}

void SWBuffer::allocMem( size_t size )
{
	freeMem();
	
	if ( size > 0 ) m_buf = SWUtil.alloc( (m_size = size) );

}
void SWBuffer::freeMem()
{
	if ( m_size > 0 ) SWUtil.free( m_buf );

	m_buf  = NULL;
	m_size = 0;
}
void* SWBuffer::getPtr()
{
	return m_buf;
}