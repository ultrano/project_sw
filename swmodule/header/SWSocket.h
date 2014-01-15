#ifndef SWSocket_h__
#define SWSocket_h__

#include "SWObject.h"
#include "SWIOStream.h"

class SWSocket : public SWObject
{
	SW_RTTI( SWSocket, SWObject );

	class Pimpl;
	SWHardRef<Pimpl> m_pimpl;

public:

	SWSocket();
	~SWSocket();
	
	SWHardRef<SWOutputStream> getOutputStream();
	SWHardRef<SWInputStream>  getInputStream();

	//! this is not for you.
	Pimpl* pimpl() const;
};

#endif // SWSocket_h__