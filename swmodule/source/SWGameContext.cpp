
#include "SWGameContext.h"
#include "SWHeaders.h"

#include <memory>
#include <stdio.h>
#include <fstream>

void registerBasicObjectFactories( SWGameContext* gc )
{
	gc->registerFactory<SWAction>();
	gc->registerFactory<SWActDelay>();
	gc->registerFactory<SWActDestroy>();
	gc->registerFactory<SWActBunch>();
	gc->registerFactory<SWActColor>();
	gc->registerFactory<SWActColorTo>();
	gc->registerFactory<SWActColorFrom>();
	gc->registerFactory<SWActColorBy>();
	gc->registerFactory<SWActSequence>();
	gc->registerFactory<SWActAnimate>();
	gc->registerFactory<SWActRepeat>();
	gc->registerFactory<SWActMove>();
	gc->registerFactory<SWActMoveTo>();
	gc->registerFactory<SWActMoveFrom>();
	gc->registerFactory<SWActMoveBy>();
	gc->registerFactory<SWActRotate>();
	gc->registerFactory<SWActRotateTo>();
	gc->registerFactory<SWActRotateFrom>();
	gc->registerFactory<SWActRotateBy>();
	gc->registerFactory<SWActScale>();
	gc->registerFactory<SWActScaleTo>();
	gc->registerFactory<SWActScaleFrom>();
	gc->registerFactory<SWActScaleBy>();
	
	gc->registerFactory<SWCamera>();
	gc->registerFactory<SWCircleCollider2D>();
	
	gc->registerFactory<SWFontRenderer>();

	gc->registerFactory<SWGameObject>();
	
	gc->registerFactory<SWMeshFilter>();
	gc->registerFactory<SWMeshRenderer>();
	gc->registerFactory<SWMesh>();
	gc->registerFactory<SWMaterial>();
	
	gc->registerFactory<SWRigidBody2D>();
	gc->registerFactory<SWRectCollider2D>();
	
	gc->registerFactory<SWSprite>();
	gc->registerFactory<SWSpriteRenderer>();
	gc->registerFactory<SWSpriteSequence>();

	gc->registerFactory<SWTransform>();
}

SWGameContext::SWGameContext()
{
}

void SWGameContext::onStart( SWGameScene* firstScene, const SWPlatformAssetsAccessor* accessor, int width, int height )
{
	m_nextScene = firstScene;
	m_nextSceneReserved = true;
	m_screenWidth  = width;
	m_screenHeight = height;

	SWAssets.m_accessor = accessor;
	SWTime.m_lastFrameTime = SWTime.getTime();

	//! opengl initializing
	{
		// 甕곌쑵�쓠 占쎄깻�뵳�딅선 占쎄퉳占쎄맒 筌욑옙占쎌젟.
		glClearColor(0,0,0,0);
		
		// 甕곌쑵�볩옙�뮞 甕곌쑵�쓠 占쎄텢占쎌뒠
		glEnable(GL_TEXTURE_2D);

		glEnable(GL_BLEND);
		//glEnable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glAlphaFunc(GL_GREATER, 0);

		//! 夷⑺슆占쎈땰吏� 泥⑺슃夷섏㏏夷섏㎟姨뷀슜

		// 占쎈떭占쎄껴�듊占쎌뭼�듊�쉱 吏뺧옙�뀒�돞占�.
		glViewport(0,0,width,height);

	}

	//! setting default shader and material
	{
		tstring source =
		"uniform mat4      MATRIX_MVP;\n"
		"uniform sampler2D TEXTURE_0;\n"
		"uniform vec4      COLOR;\n"
		"varying vec2      v_tex;\n"

		"#ifdef VERTEX_SHADER\n"

		"attribute vec4 a_pos;\n"
		"attribute vec2 a_tex;\n"

		"void main()\n"
		"{\n"
		"   gl_Position = MATRIX_MVP * a_pos;\n"
		"   v_tex = a_tex;\n"
		"}\n"

		"#endif\n"

		"#ifdef FRAGMENT_SHADER\n"

		"void main()\n"
		"{\n"
		"   gl_FragColor = COLOR * texture2D( TEXTURE_0, v_tex );\n"
		"}\n"

		"#endif\n";
		
		SWMaterial::setDefaultShader( NULL );
		SWHardRef<SWShader> defaultShader = SWShader::compileShader( source );
		SWMaterial::setDefaultShader( defaultShader() );
        SWLog( "game start up" );
	}

	//! register basic components
	{
		registerBasicObjectFactories( this );
	}
}

void SWGameContext::onFrameMove()
{
	float nowTime = SWTime.getTime();
	SWTime.m_deltaFrameTime = ( nowTime - SWTime.m_lastFrameTime );
	SWTime.m_lastFrameTime = nowTime;

	SWTime.m_accumFrame += 1;
	SWTime.m_accumTime  += SWTime.getDeltaTime();

	SWTime.m_DPS = ( SWTime.m_accumDraw / SWTime.m_accumTime );
	SWTime.m_FPS = ( SWTime.m_accumFrame / SWTime.m_accumTime );

	if ( SWTime.m_accumTime > 3 )
	{
		SWLog( "FPS : %.1f", SWTime.getFPS() );
		SWLog( "DPS : %.1f", SWTime.getDPS() );
		SWTime.m_accumDraw  /= SWTime.m_accumTime;
		SWTime.m_accumFrame /= SWTime.m_accumTime;
		SWTime.m_accumTime  /= SWTime.m_accumTime;
	}

	if ( m_nextSceneReserved )
	{
		m_nextSceneReserved = false;
		if ( m_currentScene.isValid() ) m_currentScene()->destroy();
		m_currentScene = m_nextScene();
		m_nextScene = NULL;
		if ( m_currentScene.isValid() ) m_currentScene()->awake();
	}
	
	if ( SWGameScene* scene = m_currentScene() ) scene->update();

	SWInput.m_deltaX = 0;
	SWInput.m_deltaY = 0;

	SWLogCenter::getInstance().present();
}

void SWGameContext::onRender()
{
	if ( SWGameScene* scene = m_currentScene() )
	{
		scene->draw();
	}
	SWTime.m_accumDraw += 1;
}

void SWGameContext::onTouch( int type, int param1, int param2 )
{
	SWInput.m_touchState = type;
	if ( type == SW_TouchPress )
	{
		SWInput.m_deltaX = 0;
		SWInput.m_deltaY = 0;
	}
	else if ( type == SW_TouchMove )
	{
		SWInput.m_deltaX = param1 - (int)SWInput.m_touchXY.x;
		SWInput.m_deltaY = param2 - (int)SWInput.m_touchXY.y;
	}
	SWInput.m_touchXY = tvec2( param1, param2 );

	/*/
	switch ( type )
	{
	case SW_TouchPress   : SWLog( "pressed" ); break;
	case SW_TouchRelease : SWLog( "released" ); break;
	case SW_TouchMove    : SWLog( "moved" ); break;
	}
	//*/
	//////////////////////////////////////////////////////////////////////////
	{
		SWList::Type copy = SWInput.m_listeners;
		SWList::iterator itor = copy.begin();
		for ( ; itor != copy.end() ; ++itor )
		{
			SWDelegator* del = swrtti_cast<SWDelegator>( (*itor)() );
			if ( del ) del->call();
			else SWInput.m_listeners.remove( del );
		}
	}
}

void SWGameContext::onKeyChange( tuint key, bool press )
{
	if ( key >= (tuint)SWInput.eKeyCount ) return;
	SWInput.m_keyFlags[ key ] = press;
	SWInput.m_lastKey = key;
	SWInput.m_keyDownCount += press? +1:-1;

	//SWLog( "key:%d is %s", key, press?"pressed":"released" );

	//////////////////////////////////////////////////////////////////////////
	{
		SWList::Type copy = SWInput.m_listeners;
		SWList::iterator itor = copy.begin();
		for ( ; itor != copy.end() ; ++itor )
		{
			SWDelegator* del = swrtti_cast<SWDelegator>( (*itor)() );
			if ( del ) del->call();
			else SWInput.m_listeners.remove( del );
		}
	}
}

SWGameContext& SWGameContext::getInstance()
{
	static SWGameContext s_instance;
	return s_instance;
}

SWGameScene* SWGameContext::getScene()
{
	return m_currentScene();
}

void SWGameContext::setNextScene( SWGameScene* scene )
{
	m_nextScene = scene;
	m_nextSceneReserved = true;
}

int SWGameContext::getScreenWidth()
{
	return m_screenWidth;
}

int SWGameContext::getScreenHeight()
{
	return m_screenHeight;
}


void SWGameContext::onResize( int width, int height )
{
	// 占쎈떭占쎄껴�듊占쎌뭼�듊�쉱 吏뺧옙�뀒�돞占�.
	glViewport(0,0,width,height);
}

void SWGameContext::registerFactory( const SWRtti* rtti, const SWAbstractObjectFactory* factory )
{
	if ( rtti == NULL || factory == NULL ) return;
	m_factoryTable[rtti] = factory;
}

SWHardRef<SWObject> SWGameContext::newInstance( const SWRtti* rtti )
{
	FactoryTable::iterator itor = m_factoryTable.find( rtti );
	if ( itor != m_factoryTable.end() ) return itor->second()->newInstance();
	return NULL;
}

SWHardRef<SWObject> SWGameContext::newInstance( const tstring& name )
{
	FactoryTable::iterator itor = m_factoryTable.begin();
	for ( ; itor != m_factoryTable.end() ; ++itor )
	{
		if ( name == itor->first->name )
		{
			return itor->second()->newInstance();
		}
	}
	return NULL;
}
