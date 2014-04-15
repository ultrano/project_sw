#include "SWAssets.h"
#include "SWRefCounter.h"
#include "SWFileStream.h"
#include <errno.h>

#ifdef WINDOWS

#elif OSX

#elif ANDROID
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
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
class __SWAssets::Pimpl : public SWRefCountable
{
public:
	AAssetManager* assetMgr;
	typedef ttable< thashstr, SWHardRef<SWInputStream> > AssetTable;
	AssetTable cache;
};

void __SWAssets::init( void* javaAssetMgr )
{
	m_pimpl = new Pimpl;
	m_pimpl()->assetMgr = (AAssetManager*)javaAssetMgr;
}

SWInputStream* __SWAssets::load( const tstring& filePath )
{
	if ( !m_pimpl.isValid() ) return NULL;

	SWInputStream* ret = NULL;
	Pimpl::AssetTable::iterator itor = m_pimpl()->cache.find( filePath );
	if ( itor != m_pimpl()->cache.end() )
	{
		return itor->second();
	}

	AAsset* asset = AAssetManager_open( m_pimpl()->assetMgr, filePath.c_str(), 0);
	if(asset)
	{
		FILE* file = funopen(asset, android_read, android_write, android_seek, android_close);
		ret = new SWFileInputStream( file );
	}

	return ret;
}
#endif


__SWAssets::__SWAssets()
{

}

__SWAssets::~__SWAssets()
{

}

__SWAssets& __SWAssets::getInstance()
{
	static __SWAssets instance;
	return instance;
}

