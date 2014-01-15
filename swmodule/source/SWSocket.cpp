#include "SWSocket.h"
#include "SWLog.h"

#ifdef WIN32
#include <WinSock2.h>
struct _WinSockHolder
{
	_WinSockHolder()
	{
		WSADATA wsaData;
		int retval = WSAStartup(MAKEWORD(2,2),&wsaData);
	}
	~_WinSockHolder() { WSACleanup(); }
};
#define WINSOCKHOLDER static _WinSockHolder _winsockholder;
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define WINSOCKHOLDER 1;
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
	void write(tbyte* b, tuint len)
	{
		send( m_socket()->pimpl()->sock, b, len, 0);
	}
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
	int sock;
	SWWeakRef<SWOutputStream> os;
	SWWeakRef<SWInputStream>  is;
};

SWSocket::SWSocket()
	: m_pimpl( new SWSocket::Pimpl() )
{
	WINSOCKHOLDER;
	pimpl()->sock = socket( PF_INET, SOCK_STREAM, 0 ); 
}
SWSocket::~SWSocket()
{
}

void SWSocket::connect( const tstring& ip, int port )
{
	sockaddr_in serv = {0};
	serv.sin_family  = AF_INET;
	serv.sin_port    = htons( port );
	serv.sin_addr.s_addr = inet_addr( ip.c_str() );
	
	if ( ::connect( pimpl()->sock, (sockaddr*)&serv, sizeof( serv ) ) < 0 )
	{
		SWLog( "fail to connect" );
		return;
	}
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