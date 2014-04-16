#ifndef SWGameContext_h__
#define SWGameContext_h__

#include "SWRefCounter.h"
#include "SWCriticalSection.h"
#include "SWPlatformAssetsAccessor.h"
#include "SWType.h"

#define SW_GC ( SWGameContext::getInstance() )

class SWGameScene;
class SWMaterial;
class SWShader;

class SWGameContext
{
public:

	static SWGameContext& getInstance();

	void onStart( SWGameScene* firstScene, const SWPlatformAssetsAccessor* accessor, int width, int height );
	void onFrameMove();
	void onRender();
	void onResume();
	void onPause();
	void onDestroy();
	void onResize( int width, int height );
	void onTouch( int type, int param1, int param2 );
	void onKeyChange( tuint key, bool press );

	SWGameScene* getScene();
	void setNextScene( SWGameScene* scene );

	int getScreenWidth();
	int getScreenHeight();

private:

	SWGameContext();
	SWGameContext( const SWGameContext& );

private:

	SWHardRef<SWGameScene> m_currentScene;
	SWHardRef<SWGameScene> m_nextScene;
	bool m_nextSceneReserved;

	int m_screenWidth;
	int m_screenHeight;

	SWCriticalSection m_idleSection;
};

#endif // SWGameContext_h__