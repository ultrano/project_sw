#ifndef __SWPlatformAssetsAccessor_h__
#define __SWPlatformAssetsAccessor_h__

#include "SWRefCounter.h"
#include "SWIOStream.h"
#include "SWPlatform.h"

class SWPlatformAssetsAccessor : public SWRefCountable
{
public:
	virtual SWHardRef<SWInputStream> access( const tstring& filePath ) = 0;
};

#ifdef PLATFORM_WIN32

class SWWIN32AssetsAccessor : public SWPlatformAssetsAccessor
{
public:
	SWWIN32AssetsAccessor( const tstring& assetPath ) : m_assetPath( assetPath ) {}
	SWHardRef<SWInputStream> access( const tstring& filePath );
private:
	tstring m_assetPath;
};

#elif PLATFORM_IOS

class SWIOSAssetsAccessor : public SWPlatformAssetsAccessor
{
public:
    SWIOSAssetsAccessor( NSBundle* bundle ) : m_bundle( bundle ) {};
	SWHardRef<SWInputStream> access( const tstring& filePath );
private:
    NSBundle* m_bundle;
};

#elif PLATFORM_ANDROID

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
class SWAndroidAssetsAccessor : public SWPlatformAssetsAccessor
{
public:
	SWAndroidAssetsAccessor( AAssetManager* assetMgr ) : m_assetMgr( assetMgr ) {}
	SWHardRef<SWInputStream> access( const tstring& filePath );
private:
	AAssetManager* m_assetMgr;
};

#endif

#endif //! __SWPlatformAssetsAccessor_h__
