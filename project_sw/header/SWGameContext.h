#ifndef SWGameContext_h__
#define SWGameContext_h__

#include "SWRefCounter.h"
#include <string>

#define SW_GC ( SWGameContext::getInstance() )

class SWMatrix4x4;
class SWGameScene;

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
	void onResume();
	void onPause();
	void onDestroy();
	void onHandleEvent( int type, int param1, int param2 );

	void* alloc( size_t size );
	void  free( void* memory );

	SWGameScene* getScene();

	void setViewMatrix( const SWMatrix4x4& matrix );
	void setModelMatrix( const SWMatrix4x4& matrix );
	void setProjectionMatrix( const SWMatrix4x4& matrix );

	void setVertexBuffer( const float* buffer );
	void setTexCoordBuffer( const float* buffer );
	void indexedDraw( size_t count, unsigned short* indeces);

	static SWGameContext& getInstance();
private:
	
};

#endif // SWGameContext_h__