
#ifndef __SWAssets_h_
#define __SWAssets_h_

#define SWAssets (__SWAssets::getInstance())
#include "SWPlatformAssetsAccessor.h"
#include "SWByteBuffer.h"
#include "SWTexture.h"

class __SWAssets
{
public:

	static __SWAssets& getInstance();

	SWHardRef<SWInputStream> load( const tstring& filePath );
	SWHardRef<SWTexture> loadTexture( const tstring& filePath );
private:

	__SWAssets();
	~__SWAssets();

private:
	
	friend class SWGameContext;
	typedef ttable< thashstr, SWHardRef<SWByteBuffer> > AssetTable;
	typedef ttable< thashstr, SWHardRef<SWTexture> > TextureTable;

	SWHardRef<SWPlatformAssetsAccessor> m_accessor;
	AssetTable m_cache;
	TextureTable m_texCache;
};


#endif //!__SWAssets_h_
