#include "SWSocket.h"
#include "SWMath.h"
#include "SWLog.h"
#include "SWPlatform.h"
#include <fcntl.h>

#ifdef PLATFORM_WIN32

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <io.h>
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
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <unistd.h>
#define WINSOCKHOLDER 1;

#endif

class SWSocket::Pimpl : public SWRefCountable
{
	friend class SWSocketOutputStream;
	friend class SWSocketInputStream;
public:
	int sock;
	SWWeakRef<SWOutputStream> os;
	SWWeakRef<SWInputStream>  is;
	bool connected;
};

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
		if ( b == NULL ) return;
		if ( len == 0 ) return;
		if ( m_socket.isValid() == false ) return;
		if ( m_socket()->isConnected() == false ) return;

		int ret = send( m_socket()->pimpl()->sock, (const char*)b, len, 0);
		if ( ret < 0 ) SWLog( "fail to send" );
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
	int read(tbyte* b, tuint len)
	{
		if ( b == NULL ) return 0;
		if ( len == 0 ) return 0;
		if ( m_socket.isValid() == false ) return -1;
		if ( m_socket()->isConnected() == false ) return -1;
		
		len = recv( m_socket()->pimpl()->sock, (char*)&b[0], len, 0 );
		int err = errno;
		if ( len < 0 )
		{
			if ((err == EAGAIN) || (err == EWOULDBLOCK))
			{
				len = 0;
				SWLog( "pendding recv" );
			}
			else SWLog( "fail to recv" );
		}
		return len;
	}
	int skip( tuint len )
	{
		tarray<tbyte> buf;
		buf.resize( len );
		return read( &buf[0], len );
	}
	int available()
	{
		if ( m_socket.isValid() == false ) return -1;
		if ( m_socket()->isConnected() == false ) return -1;
		return 0;
	}
};

SWSocket::SWSocket()
	: m_pimpl( new SWSocket::Pimpl() )
{
	WINSOCKHOLDER;
	pimpl()->sock = socket( AF_INET, SOCK_STREAM, 0 ); 
	pimpl()->os = NULL;
	pimpl()->is = NULL;
	pimpl()->connected = false;
}
SWSocket::~SWSocket()
{
#ifdef PLATFORM_WIN32
	closesocket( pimpl()->sock );
#else
    close( pimpl()->sock );
#endif
	m_pimpl = NULL;
}

SWHardRef<SWSocket> SWSocket::connect( const tstring& ip, int port, bool blocking )
{
	sockaddr_in serv = {0};
	serv.sin_family  = AF_INET;
	serv.sin_port    = htons( port );
	serv.sin_addr.s_addr = inet_addr( ip.c_str() );
	
	SWHardRef<SWSocket> socket = new SWSocket();

	SWSocket::Pimpl* pimpl = socket()->pimpl();

	int ret = ::connect( pimpl->sock, (sockaddr*)&serv, sizeof( serv ) );
	pimpl->connected = ( ret == 0 );
	if ( pimpl->connected == false ) SWLog( "fail to connect" );
	else
	{
		bool ret = false;
#ifdef PLATFORM_WIN32
		unsigned long mode = blocking ? 0 : 1;
		ret = (ioctlsocket(pimpl->sock, FIONBIO, &mode) == 0);
#else
		int flags = fcntl( pimpl->sock, F_GETFL, 0);
		if (flags < 0) return NULL;
		flags = blocking ? (flags&~O_NONBLOCK) : (flags|O_NONBLOCK);
		ret = (fcntl(pimpl->sock, F_SETFL, flags) == 0);
#endif
		if ( !ret )
		{
			SWLog( blocking? "fail to block socket":"fail to non-block socket" );
		}
	}
	return socket();
}

bool SWSocket::isConnected() const
{
	return pimpl()->connected;
}

SWHardRef<SWOutputStream> SWSocket::getOutputStream()
{
	SWHardRef<SWOutputStream> os = pimpl()->os();
	if ( false == os.isValid() )
	{
		os = new SWSocketOutputStream( this );
		pimpl()->os = os();
	}
	return os();
}
SWHardRef<SWInputStream>  SWSocket::getInputStream()
{
	SWHardRef<SWInputStream> is = pimpl()->is();
	if ( false == is.isValid() )
	{
		is = new SWSocketInputStream( this );
		pimpl()->is = is();
	}
	return is();
}

SWSocket::Pimpl* SWSocket::pimpl() const
{
	return m_pimpl();
}