#include "SWSocket.h"

#ifdef WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#endif

class SWSocketOutputStream : public SWOutputStream
{
	SW_RTTI( SWSocketOutputStream, SWOutputStream );

	SWHardRef<SWSocket> m_socket;
	
public:
	
	SWSocketOutputStream( const SWSocket* socket )
		: m_socket( socket )
	{
	}
	void write(tbyte* b, tuint len);
};

class SWSocketInputStream : public SWInputStream
{
	SW_RTTI( SWSocketInputStream, SWInputStream );
	
	SWHardRef<SWSocket> m_socket;

public:
	
	SWSocketInputStream( const SWSocket* socket )
		: m_socket( socket )
	{
	}
	/* return : read byte count. return -1 if there is no more */
	int read(tbyte* b, tuint len);
	int skip( tuint len );
	tuint available();
};

class SWSocket::Pimpl : public SWRefCountable
{
	friend class SWSocketOutputStream;
	friend class SWSocketInputStream;
public:
	sockaddr_in sock;
	SWWeakRef<SWOutputStream> os;
	SWWeakRef<SWInputStream>  is;
};

SWSocket::SWSocket()
	: m_pimpl( new SWSocket::Pimpl() )
{
}
SWSocket::~SWSocket()
{
}

SWHardRef<SWOutputStream> SWSocket::getOutputStream()
{
	if ( false == pimpl()->os.isValid() ) pimpl()->os = new SWSocketOutputStream( this );
	return pimpl()->os();
}
SWHardRef<SWInputStream>  SWSocket::getInputStream()
{
	if ( false == pimpl()->is.isValid() ) pimpl()->is = new SWSocketInputStream( this );
	return pimpl()->is();
}

SWSocket::Pimpl* SWSocket::pimpl() const
{
	return m_pimpl();
}