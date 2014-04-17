#include "SWPlatformAssetsAccessor.h"

#ifdef WIN32

#include "SWFileStream.h"
SWHardRef<SWInputStream> SWWIN32AssetsAccessor::access( const tstring& filePath )
{
	return new SWFileInputStream( m_assetPath + filePath );
}

#elif TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE

#include "SWFileStream.h"
SWHardRef<SWInputStream> SWIOSAssetsAccessor::access( const tstring& filePath )
{
    NSString* manifest_string = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:filePath.c_str()]
                                                                ofType:nil];
    const char* manifest_path = [manifest_string fileSystemRepresentation];
    
    FILE* file = fopen(manifest_path, "r");
	return new SWFileInputStream( file );
}

#elif ANDROID

#include "SWFileStream.h"
#include <errno.h>

int android_read(void* cookie, char* buf, int size)
{
  return AAsset_read((AAsset*)cookie, buf, size);
}

int android_write(void* cookie, const char* buf, int size)
{
  return EACCES; // can't provide write access to the apk
}

fpos_t android_seek(void* cookie, fpos_t offset, int whence)
{
  return AAsset_seek((AAsset*)cookie, offset, whence);
}

int android_close(void* cookie)
{
  AAsset_close((AAsset*)cookie);
  return 0;
}

SWHardRef<SWInputStream> SWAndroidAssetsAccessor::access( const tstring& filePath )
{
	AAsset* asset = AAssetManager_open( m_assetMgr, filePath.c_str(), 0);
	if( asset == NULL ) return NULL;

	FILE* file = funopen(asset, android_read, android_write, android_seek, android_close);
	return new SWFileInputStream( file );
}

#endif
