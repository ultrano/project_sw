
#ifndef __SWAssets_h_
#define __SWAssets_h_

#include "SWType.h"
#include "SWIOStream.h"
#define SWAssets (__SWAssets::getInstance())

class __SWAssets
{
public:

#ifdef WINDOWS

#elif OSX

#elif ANDROID
	void init( void* javaAssetMgr );
#endif

	static __SWAssets& getInstance();

	SWInputStream* load( const tstring& filePath );

private:

	__SWAssets();
	~__SWAssets();

private:

	class Pimpl;
	SWHardRef<Pimpl> m_pimpl;

};

#endif //!__SWAssets_h_
