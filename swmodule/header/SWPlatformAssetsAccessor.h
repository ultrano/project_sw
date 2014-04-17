#ifndef __SWPlatformAssetsAccessor_h__
#define __SWPlatformAssetsAccessor_h__

#include "SWRefCounter.h"
#include "SWIOStream.h"
#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

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
#elif TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
class SWIOSAssetsAccessor : public SWPlatformAssetsAccessor
{
public:
    SWIOSAssetsAccessor( NSBundle* bundle ) : m_bundle( bundle ) {};
	SWHardRef<SWInputStream> access( const tstring& filePath );
private:
    NSBundle* m_bundle;
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