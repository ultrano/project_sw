#ifndef SWGameContext_h__
#define SWGameContext_h__

#include "SWRefCounter.h"
#include <string>
class SWGameScene;

#define SW_GC ( SWGameContext::getInstance() )

class SWMatrix4x4;
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

	void setModelViewMatrix( const SWMatrix4x4& matrix );
	void setProjectionMatrix( const SWMatrix4x4& matrix );

	//void drawImageRegion( const int& img, float dstX, float dstY, float dstWidth, float dstHeight, float srcX, float srcY, float srcWidth, float srcHeight );
	void drawFillRect( float x, float y, float width, float height );

	static SWGameContext& getInstance();

private:
	
};

#endif // SWGameContext_h__