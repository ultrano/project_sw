#include "SWIOStream.h"

SWOutputStreamWriter::SWOutputStreamWriter( SWOutputStream* os ) : m_os( os )
{

}

void SWOutputStreamWriter::writeBytes( tbyte* buffer, tuint size )
{
	if ( m_os.isValid()) m_os()->write(buffer, size);
}

void SWOutputStreamWriter::writeString( const tstring& str )
{
	if ( !m_os.isValid() ) return ;

	tuint size = str.size();
	if ( size ) m_os()->write( (tbyte*)&str[0], size );
	m_os()->writeByte( '\0' );
}

void SWOutputStreamWriter::wirteLine( const tstring& str )
{
	if ( !m_os.isValid() ) return ;

	tuint size = str.find( '\n' );
	if ( size == tstring::npos ) size = str.size();

	if ( size ) m_os()->write( (tbyte*)&str[0], size );
	m_os()->writeByte( '\n' );
}

//////////////////////////////////////////////////////////////////////////

SWInputStreamReader::SWInputStreamReader( SWInputStream* is ) : m_is(is)
{

}

int SWInputStreamReader::readBytes( tbyte* buffer, tuint size )
{
	if ( buffer == NULL ) return 0;
	if ( size == 0 ) return 0;
	if (!m_is.isValid()) return -1;
	return m_is()->read(buffer, size);
}

int SWInputStreamReader::readString( tstring& str )
{
	str.clear();
	if ( !m_is.isValid() ) return -1;

	int c;
	while ( true )
	{
		c = m_is()->readByte();
		if ( c <= 0 ) break;
		str += (char)c;
	}

	tuint size = str.size();
	return (size > 0)? size : c;
}

int SWInputStreamReader::readLine( tstring& str )
{
	str.clear();
	if ( !m_is.isValid() ) return -1;

	int c;
	while( true )
	{
		c = m_is()->readByte();
		if ( c == '\r' ) continue;
		if ( c <= 0 || c == '\n' ) break;
		str += (char)c;
	}

	tuint size = str.size();
	return (size > 0)? size : c;
}
