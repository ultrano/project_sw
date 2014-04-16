#ifndef __SWPlatformAssetsAccessor_h__
#define __SWPlatformAssetsAccessor_h__

#include "SWRefCounter.h"
#include "SWIOStream.h"

class SWPlatformAssetsAccessor : public SWRefCountable
{
public:
	virtual SWHardRef<SWInputStream> access( const tstring& filePath ) = 0;
};

#ifdef WIN32
class SWWIN32AssetsAccessor : public SWPlatformAssetsAccessor
{
public:
	SWWIN32AssetsAccessor( const tstring& assetPath ) : m_assetPath( assetPath ) {}
	SWHardRef<SWInputStream> access( const tstring& filePath );
private:
	tstring m_assetPath;
};
#elif ANDROID
class SWAndroidAssetsAccessor : public SWPlatformAssetsAccessor
{
public:
	SWAndroidAssetsAccessor( AAssetManager* assetMgr ) : m_assetMgr( assetMgr ) {}
	SWHardRef<SWInputStream> access( const tstring& filePath );
private:
	AAssetManager m_assetMgr;
};
#endif

#endif //! __SWPlatformAssetsAccessor_h__