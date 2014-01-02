#ifndef SWGameContext_h__
#define SWGameContext_h__

#include "SWRefCounter.h"
#include "SWType.h"

#define SW_GC ( SWGameContext::getInstance() )

class SWMatrix4x4;
class SWGameScene;
class SWObject;

class SWGameContext
{
	class Pimpl;
	SWHardRef<Pimpl> m_pimpl;
private:
	SWGameContext();
	SWGameContext( const SWGameContext& );
public:

	void onStart( SWGameScene* firstScene, const tstring& resFolder, int width, int height );
	void onFrameMove();
	void onRender();
	void onResume();
	void onPause();
	void onDestroy();
	void onResize( int width, int height );
	void onHandleEvent( int type, int param1, int param2 );

	void setViewMatrix( const SWMatrix4x4& matrix );
	void setModelMatrix( const SWMatrix4x4& matrix );
	void setProjectionMatrix( const SWMatrix4x4& matrix );
	void setTextureMatrix( const SWMatrix4x4& matrix );

	void setVertexBuffer( const float* buffer );
	void setTexCoordBuffer( const float* buffer );
	void indexedDraw( size_t count, unsigned short* indeces);
	
	SWHardRef<SWObject> loadJson( const tstring& path );
	SWHardRef<SWObject> loadJsonFromString( const tstring& doc );
	unsigned int loadTexture( const tstring& path );
	unsigned int loadTextureFromMemory( const unsigned char* buf, size_t len );
	bool getTextureSize( int texID, int& width, int& height );
	void bindTexture( unsigned int texID );

	SWGameScene* getScene();
	void setNextScene( SWGameScene* scene );

	int getScreenWidth();
	int getScreenHeight();

	bool storeItem( const tstring& key, SWObject* item );
	void removeItem( const tstring& key );
	SWObject* findItem( const tstring& key );

	static SWGameContext& getInstance();
private:
	
};

#endif // SWGameContext_h__