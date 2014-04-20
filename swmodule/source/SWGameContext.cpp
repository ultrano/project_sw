
#include "SWGameContext.h"
#include "SWOpenGL.h"
#include "SWDefines.h"

#include "SWTime.h"
#include "SWInput.h"
#include "SWLog.h"
#include "SWAssets.h"

#include "SWGameScene.h"
#include "SWShader.h"
#include "SWMaterial.h"
#include "SWCamera.h"

#include <memory>
#include <stdio.h>
#include <fstream>

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

	if ( SWGameScene* scene = m_currentScene() )
	{
		scene->update();
	}

	SWInput.m_deltaX = 0;
	SWInput.m_deltaY = 0;
	SWInput.m_touchState = SW_TouchNone;

	SWLogCenter::getInstance().present();
}

void SWGameContext::onRender()
{
	if ( SWCamera::mainCamera.isValid() == false ) return;

	const tcolor& color = SWCamera::mainCamera()->getClearColor();
	glClearColor( color.r, color.g, color.b, color.a );
	glClear( GL_COLOR_BUFFER_BIT );
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
		SWInput.m_deltaX = param1;
		SWInput.m_deltaY = param2;
	}
	else if ( type == SW_TouchMove )
	{
		SWInput.m_deltaX = param1 - SWInput.m_touchX;
		SWInput.m_deltaY = param2 - SWInput.m_touchY;
	}
	SWInput.m_touchX = param1;
	SWInput.m_touchY = param2;

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
	if ( key >= SWInput.eKeyCount ) return;
	SWInput.m_keyFlags[ key ] = press;
	SWInput.m_lastKey = key;
	SWInput.m_keyDownCount += press? +1:-1;

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