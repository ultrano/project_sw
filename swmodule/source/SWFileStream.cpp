#include "SWFileStream.h"
#include "SWMath.h"

SWFileOutputStream::SWFileOutputStream()
	: m_file(NULL)
{

}

SWFileOutputStream::SWFileOutputStream( const tstring& file )
	: m_file(NULL)
{
	open(file);
}

SWFileOutputStream::~SWFileOutputStream()
{
	close();
}

void SWFileOutputStream::write( tbyte* b, tuint len )
{

	if ( m_file ) fwrite( b, len, 1, m_file );
}

bool SWFileOutputStream::open( const tstring& file )
{
	close();
	m_file = fopen( file.c_str(),"wb" );
	return !!m_file;
}

void SWFileOutputStream::close()
{
	if (m_file)
	{
		fclose(m_file);
		m_file = NULL;
	}
}

tuint SWFileOutputStream::size()
{
	fpos_t cur = 0;
	fgetpos( m_file, &cur );
	fpos_t sz = fseek(m_file, 0, SEEK_END);
	fgetpos( m_file, &sz );
	fsetpos( m_file, &cur );
	return (tuint)sz;
}

void SWFileOutputStream::setPos( tuint pos )
{
	fsetpos( m_file, (fpos_t*)&pos );
}

tuint SWFileOutputStream::getPos()
{
	fpos_t cur = 0;
	fgetpos( m_file, &cur );
	return (tuint)cur;
}

FILE* SWFileOutputStream::getFile()
{
	return m_file;
}

//////////////////////////////////////////////////////////////////////////


SWFileInputStream::SWFileInputStream()
	: m_file(NULL)
{

}

SWFileInputStream::SWFileInputStream( const tstring& file )
	: m_file(NULL)
{
	open(file);
}

SWFileInputStream::~SWFileInputStream()
{
	close();
}

int SWFileInputStream::read( tbyte* b, unsigned len )
{
	if ( m_file )
	{
		fpos_t pos0 = 0, pos1 = 0;
		fgetpos( m_file, &pos0 );
		fread( b, len, 1, m_file );
		fgetpos( m_file, &pos1 );
		return (tuint)((pos1-pos0)? (pos1-pos0):-1);
	}
	return -1;
}

int SWFileInputStream::available()
{
	if ( m_file == NULL ) return -1;
	fpos_t cur = 0;
	fgetpos( m_file, &cur );
	fpos_t sz = fseek(m_file, 0, SEEK_END);
	fgetpos( m_file, &sz );
	fsetpos( m_file, &cur );
	return (tuint)sz;
}
int SWFileInputStream::skip( tuint len )
{
	tuint remain = available() - getPos();
	len = SWMath.min( remain, len );
	fseek(m_file, len, SEEK_CUR);
	return len;
}

bool SWFileInputStream::open( const tstring& file )
{
	close();
	m_file = fopen( file.c_str(),"rb" );
	return !!m_file;
}

void SWFileInputStream::close()
{
	if (m_file)
	{
		fclose(m_file);
		m_file = NULL;
	}
}

void SWFileInputStream::setPos( tuint pos )
{
	fsetpos( m_file, (fpos_t*)&pos );
}

tuint SWFileInputStream::getPos()
{
	fpos_t cur = 0;
	fgetpos( m_file, &cur );
	return (tuint)cur;
}

FILE* SWFileInputStream::getFile()
{
	return m_file;
}
