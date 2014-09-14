#ifndef SWGameContext_h__
#define SWGameContext_h__

#include "SWRefCounter.h"
#include "SWCriticalSection.h"
#include "SWPlatformAssetsAccessor.h"
#include "SWType.h"
#include "SWObjectFactory.h"

#define SW_GC ( SWGameContext::getInstance() )

class SWGameScene;
class SWMaterial;
class SWShader;
class SWObject;
class SWAbstractObjectFactory;

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

	template<typename T>
	void registerFactory() { registerFactory( T::getRtti(), new SWTemplateObjectFactory<T>() ); }
	void registerFactory( const SWRtti* rtti, const SWAbstractObjectFactory* factory );

	template<typename T>
	SWHardRef<T> newInstance()
	{
		SWHardRef<SWObject> inst = newInstance( T::getRtti() );
		return swrtti_cast<T>( inst() );
	}

	template<typename T>
	SWHardRef<T> newInstance( const tstring& name )
	{
		SWHardRef<SWObject> inst = newInstance( name );
		return swrtti_cast<T>( inst() );
	}

	SWHardRef<SWObject> newInstance( const SWRtti* rtti );
	SWHardRef<SWObject> newInstance( const tstring& name );

private:

	SWGameContext();
	SWGameContext( const SWGameContext& );

private:

	typedef ttable<const SWRtti*, SWHardRef<SWAbstractObjectFactory> > FactoryTable;

	SWHardRef<SWGameScene> m_currentScene;
	SWHardRef<SWGameScene> m_nextScene;
	bool m_nextSceneReserved;

	int m_screenWidth;
	int m_screenHeight;

	SWCriticalSection m_idleSection;

	FactoryTable m_factoryTable;
};

#endif // SWGameContext_h__