#ifndef SWSocket_h__
#define SWSocket_h__

#include "SWObject.h"
#include "SWIOStream.h"

class SWSocket : public SWObject
{
	SW_RTTI( SWSocket, SWObject );
	class Pimpl;

public:
	
	static SWHardRef<SWSocket> connect( const tstring& ip, int port, bool blocking = false );
	bool isConnected() const;

	SWHardRef<SWOutputStream> getOutputStream();
	SWHardRef<SWInputStream>  getInputStream();

	//! this is not for you.
	Pimpl* pimpl() const;

private:

	SWSocket();
	~SWSocket();

private:

	SWHardRef<Pimpl> m_pimpl;

};

#endif // SWSocket_h__