#ifndef SWGameContext_h__
#define SWGameContext_h__

#include "SWRefCounter.h"
#include <string>

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

	void onStart( SWGameScene* firstScene, const std::string& resFolder, float width, float height );
	void onFrameMove();
	void onRender();
	void onResume();
	void onPause();
	void onDestroy();
	void onHandleEvent( int type, int param1, int param2 );
	void onResize( int width, int height );

	void* alloc( size_t size );
	void  free( void* memory );

	SWGameScene* getScene();
	void setNextScene( SWGameScene* scene );

	float deltaTime() const;
	float awakeTime() const;

	bool storeItem( const std::string& key, SWObject* item );
	void removeItem( const std::string& key );
	SWObject* findItem( const std::string& key );

	int  getTouchState() const;
	int  getTouchX() const;
	int  getTouchY() const;

	void setViewMatrix( const SWMatrix4x4& matrix );
	void setModelMatrix( const SWMatrix4x4& matrix );
	void setProjectionMatrix( const SWMatrix4x4& matrix );

	void setVertexBuffer( const float* buffer );
	void setTexCoordBuffer( const float* buffer );
	void indexedDraw( size_t count, unsigned short* indeces);

	unsigned int loadTexture( const std::string& path );
	void bindTexture( unsigned int texID );

	static SWGameContext& getInstance();
private:
	
};

#endif // SWGameContext_h__