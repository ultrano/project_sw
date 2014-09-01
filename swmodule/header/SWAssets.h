
#ifndef __SWAssets_h_
#define __SWAssets_h_

#define SWAssets (__SWAssets::getInstance())
#include "SWPlatformAssetsAccessor.h"
#include "SWByteBuffer.h"
#include "SWTexture.h"
#include "SWShader.h"
#include "SWSpriteAtlas.h"
#include "SWSpriteAnimation.h"
#include "SWFontInfo.h"

class __SWAssets
{
public:

	static __SWAssets& getInstance();

	SWHardRef<SWInputStream> loadBuffer( const tstring& filePath );
	SWHardRef<SWTexture> loadTexture( const tstring& filePath );
	SWHardRef<SWShader>  loadShader( const tstring& filePath );
	SWHardRef<SWSpriteAnimation> loadSpriteAnimation( const tstring& filePath );
	SWHardRef<SWSpriteAtlas> loadSpriteAtlas( const tstring& filePath );
	SWHardRef<SWFontInfo> loadFontInfo( const tstring& filePath );

	bool findPathOfTexture( SWTexture* texture, tstring& path );
	bool findPathOfShader( SWShader* shader, tstring& path );
private:

	__SWAssets();
	~__SWAssets();

private:
	
	friend class SWGameContext;
	typedef ttable< thashstr, SWWeakRef<SWByteBuffer> > BufferTable;
	typedef ttable< thashstr, SWWeakRef<SWTexture> > TextureTable;
	typedef ttable< thashstr, SWWeakRef<SWShader> >  ShaderTable;
	typedef ttable< thashstr, SWWeakRef<SWSpriteAtlas> >  AtlasTable;
	typedef ttable< thashstr, SWWeakRef<SWSpriteAnimation> >  AnimationTable;
	typedef ttable< thashstr, SWWeakRef<SWFontInfo> >  FontInfoTable;

	SWHardRef<SWPlatformAssetsAccessor> m_accessor;
	BufferTable   m_bufferCache;
	TextureTable  m_texCache;
	ShaderTable   m_shaderCache;
	AtlasTable    m_atlasCache;
	AnimationTable m_animCache;
	FontInfoTable m_fontInfoCache;
};


#endif //!__SWAssets_h_
