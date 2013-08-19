//
//  SWGameContext.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWGameContext.h"
#include "SWGameScene.h"
#include "SWLog.h"
#include "SWSpriteData.h"

#include <math.h>

void frameLoop( CSTimer* tm, void* param )
{
    SW_GC.onFrameMove();
    CS_knlSetTimer( tm, 10, param );
}

SWGameContext& SWGameContext::getInstance()
{
    static SWGameContext s_instance;
    return s_instance;
}

void SWGameContext::onStart( SWGameScene* firstScene, const std::string& resFolder )
{
    
    SW_OutputLog( "context", "app is started" );
    
    CS_grpInitContext( &graphicContext );
    CS_knlDefTimer( &loopTimer, &frameLoop );
    mainFrameBuffer = CS_grpGetScreenFrameBuffer( 0 );
    screenWidth     = CS_GRP_GET_FRAME_BUFFER_WIDTH( mainFrameBuffer );
    screenHeight    = CS_GRP_GET_FRAME_BUFFER_HEIGHT( mainFrameBuffer );
    worldWidth      = screenWidth;
    worldHeight     = screenHeight;
    fps             = 0;
    fpsAccumTime    = 0;
    fpsFrameCount   = 0;
    fixedFrameDrawing = 3;
    currentScene    = NULL;
    nextScene       = firstScene;
    resourcePathBase = resFolder;
    
    onResume();
    frameLoop( &loopTimer, NULL );
}

void SWGameContext::onDestroy()
{
    /////////////////////////////
    m_cs.wait();
    SWSectionGuard ____sg(m_cs);
    /////////////////////////////
    SW_OutputLog( "context", "app is destroied" );
    
    onPause();
    CS_knlUnsetTimer( &loopTimer );
    if ( currentScene.isValid() ) currentScene()->onDestroy();
    currentScene = NULL;
    nextScene    = NULL;
    
}

void SWGameContext::onFrameMove()
{
    /////////////////////////////
    m_cs.wait();
    SWSectionGuard ____sg(m_cs);
    /////////////////////////////
    
    M_Int32 newFrameTime = CS_knlCurrentTime();
    M_Int32 elapsedTime = newFrameTime - lastFrameTime;
    
    fpsAccumTime += elapsedTime;
    ++fpsFrameCount;
    if ( fpsAccumTime > 3000 )
    {
        fps = ( (float)fpsFrameCount / (float)fpsAccumTime ) * 1000.0f;
        fpsAccumTime = 0;
        fpsFrameCount = 0;
    }
    
    if ( nextScene.isValid() )
    {
        if ( currentScene.isValid() ) currentScene()->destroy();
        currentScene = nextScene();
        nextScene = NULL;
        if ( currentScene.isValid() ) currentScene()->awake();
    }
    
    if ( currentScene.isValid() )
    {
        currentScene()->update( ( (float)elapsedTime ) / 1000.0f );
        
        if ( ( fpsFrameCount % fixedFrameDrawing ) == 0 )
        {
            //! 랜더링 시작.
            CS_grpBegin();
            
            //! 화면 전체를 지정 색으로 클리어.
            clearFrame( 0, 0, 255 );
            
            //! 씬 렌더링
            currentScene()->draw();
            
            //! 랜더링 하느라 트러진 프레임을 원위치.
            setScale( 1, 1 );
            setRotate( 0 );
            setTranslate( 0, 0 );
            setAlpha( 1 );
            setRGB( 255, 255, 255 );
            
            //! 랜더링 종료.
            CS_grpEnd();
            
            //! 랜더링을 스크린으로 송신.
            CS_grpFlushLcd( 0, mainFrameBuffer, 0, 0, screenWidth, screenHeight );
            
        }
    }
    lastFrameTime = newFrameTime;
}

void SWGameContext::onResume()
{
    /////////////////////////////
    m_cs.wait();
    SWSectionGuard ____sg(m_cs);
    /////////////////////////////
    
    SoundTable::iterator itor = soundCacheTable.begin();
    for ( ; itor != soundCacheTable.end() ; ++itor ) resumeSound( (itor->second) );
    
    SW_OutputLog( "context", "app is resumed" );
    lastFrameTime = CS_knlCurrentTime();
    CS_knlSetTimer( &loopTimer, 1, NULL );
    if ( currentScene.isValid() ) currentScene()->onResume();
    
}

void SWGameContext::onPause()
{
    /////////////////////////////
    m_cs.wait();
    SWSectionGuard ____sg(m_cs);
    /////////////////////////////
    
    SoundTable::iterator itor = soundCacheTable.begin();
    for ( ; itor != soundCacheTable.end() ; ++itor ) pauseSound( (itor->second) );

    SW_OutputLog( "context", "app is paused" );
    
    if ( currentScene.isValid() ) currentScene()->onPause();
    CS_knlUnsetTimer( &loopTimer );

}

void SWGameContext::onHandleEvent( int type, int param1, int param2 )
{
    /////////////////////////////
    m_cs.wait();
    SWSectionGuard ____sg(m_cs);
    /////////////////////////////
    
	// application에 evnet가 들어 올때 호출 되는 부분입니다.
    //! press, release, drag(move) 에 관한 같은 종류의
    //! 다른 표현 3개가 연속으로 들어오기 때문에
    //! 그중 한개만 걸러서 처리. (예. press가 연속으로 3개가 들어옴)
    //onHandleEvent1( type, param1, param2 );
    //onHandleEvent2( type, param1, param2 );
    //onHandleEvent3( type, param1, param2 );

    if ( currentScene.isValid() && onHandleEvent3( type, param1, param2 ) )
    {
        currentScene()->handleEvent( touchEventType, touchedPoint2[0], touchedPoint2[1] );
    }
}

bool SWGameContext::onHandleEvent1( int type, int param1, int param2 )
{
	// event로 MV_POINTER_XXX define 에 해당하는 type이 들어 올 경우, 그 define에 대한 좌표 값을 가져 옵니다.
	if( (type >= MV_POINTER_PRESS_EVENT) && (type <= MV_POINTER_MOVE_EVENT) )
	{
		touchedPoint2[0] = ((int*)param1)[0];
		touchedPoint2[1] = ((int*)param1)[1];
        
        // wrapper의 같은 형태의 타입을 게임에서 타루는 타입으로 통일.
        switch ( type )
        {
            case MV_POINTER_PRESS_EVENT :
                touchEventType = SW_TouchPress;
                break;
            case MV_POINTER_RELEASE_EVENT:
                touchEventType = SW_TouchRelease;
                break;
            case MV_POINTER_MOVE_EVENT :
                touchEventType = SW_TouchMove;
                break;
        }
        
        lastTouchedPoint2[0] = touchedPoint2[0];
        lastTouchedPoint2[1] = touchedPoint2[1];
        
        return true;
	}
    return false;
}

bool SWGameContext::onHandleEvent2( int type, int param1, int param2 )
{
    // event로 CS_APP_EVENT_XXX define 에 해당하는 type이 들어 올 경우, 그 define에 대한 좌표 값을 가져 옵니다.
    if( (type >= CS_APP_EVENT_TOUCH) && (type <= CS_APP_EVENT_RELEASE_TOUCH) )
	{
        // CS_APP_EVENT_DRAG 의 경우 이전 좌표의 상대 좌표이기 때문에 특별 처리
        if ( CS_APP_EVENT_DRAG == type )
        {
            touchedPoint2[0] = lastTouchedPoint2[0] + param1;
            touchedPoint2[1] = lastTouchedPoint2[1] + param2;
        }
        else
        {
            touchedPoint2[0] = param1;
            touchedPoint2[1] = param2;
        }
        
        // wrapper의 같은 형태의 타입을 게임에서 타루는 타입으로 통일.
        switch ( type )
        {
            case (int)CS_APP_EVENT_TOUCH :
                touchEventType = SW_TouchPress;
                break;
            case (int)CS_APP_EVENT_RELEASE_TOUCH:
                touchEventType = SW_TouchRelease;
                break;
            case (int)CS_APP_EVENT_DRAG :
                touchEventType = SW_TouchMove;
                break;
        }
        
        lastTouchedPoint2[0] = touchedPoint2[0];
        lastTouchedPoint2[1] = touchedPoint2[1];
        
        return true;
	}
    return false;
}

bool SWGameContext::onHandleEvent3( int type, int param1, int param2 )
{
    // event로 CS_APP_EVENT_XXX_EX define 에 해당하는 type이 들어 올 경우, 그 define에 대한 좌표 값을 가져 옵니다.
    if( (type >= CS_APP_EVENT_TOUCH_EX) && (type <= CS_APP_EVENT_RELEASE_TOUCH_EX) )
	{
		touchedPoint2[0] = (int)((M_Uint32)param2>>16);
		touchedPoint2[1] = (int)((M_Uint32)param2&0xffff);
        
        // wrapper의 같은 형태의 타입을 게임에서 타루는 타입으로 통일.
        switch ( type )
        {
            case (int)CS_APP_EVENT_TOUCH_EX :
                touchEventType = SW_TouchPress;
                break;
            case (int)CS_APP_EVENT_RELEASE_TOUCH_EX :
                touchEventType = SW_TouchRelease;
                break;
            case (int)CS_APP_EVENT_DRAG_EX :
                touchEventType = SW_TouchMove;
                break;
        }
        
        lastTouchedPoint2[0] = touchedPoint2[0];
        lastTouchedPoint2[1] = touchedPoint2[1];
        
        return true;
	}
    
    return false;
}

void SWGameContext::setNextScene( SWGameScene *scene )
{
    nextScene = scene;
}

void* SWGameContext::alloc( size_t size )
{
    M_Int32 id = CS_knlAlloc( size + sizeof(M_Int32) );
    void* memory = CS_GETDPTR( id );
    *((M_Int32*)memory) = id;
    return (void*)( ( (unsigned char*)memory ) + sizeof(M_Int32) );
}

void  SWGameContext::free( void* memory )
{
    M_Int32 id = *( (M_Int32*)( ( (unsigned char*)memory ) - sizeof(M_Int32) ) );
    CS_knlFree( id );
}

float SWGameContext::getFPS()
{
    return fps;
}
int SWGameContext::getRGB()
{
    int rgb = 0;
    CS_grpGetContext( &graphicContext, CS_GRP_CONTEXT_FG_PIXEL_IDX, &rgb );
    return rgb;
}

void SWGameContext::setRGB( int rgb )
{
    CS_grpSetContext( &graphicContext, CS_GRP_CONTEXT_FG_PIXEL_IDX, (void*)rgb );
}

void SWGameContext::setRGB( unsigned char r, unsigned char g, unsigned char b )
{
    CS_grpSetContext( &graphicContext, CS_GRP_CONTEXT_FG_PIXEL_IDX, (void*)CS_grpGetPixelFromRGB( r, g, b ) );
}

bool SWGameContext::createImage( CS_GrpImage* img, const char* filePath )
{
    *img = 0;
    if ( img == NULL ) return false;
    std::string fileFullPath = resourcePathBase + filePath;
    ImageTable::iterator itor = imageCacheTable.find( fileFullPath );
    if ( itor != imageCacheTable.end() )
    {
        *img = (itor->second);
        return true;
    }
	M_Int32 resID, resSize, bufId;
	resID = CS_knlGetResourceID( (M_Char*)fileFullPath.c_str(), &resSize );
	if( resID < 0 ) return false;
	bufId = CS_knlAlloc(resSize);
	
	CS_knlGetResource(resID , (M_Uint32)(void*)bufId , resSize);
	
    CS_grpCreateImage( img, bufId, 0, resSize );
    
    imageCacheTable.insert( std::make_pair( fileFullPath, *img ) );
    return true;
}

void SWGameContext::destroyImage( const CS_GrpImage& img )
{
    if ( img ) CS_grpDestroyImage( img );
}

int  SWGameContext::getImageWidth( const CS_GrpImage& img )
{
    return CS_GRP_GET_FRAME_BUFFER_WIDTH( CS_grpGetImageFrameBuffer( img ) );
}

int  SWGameContext::getImageHeight( const CS_GrpImage& img )
{
    return CS_GRP_GET_FRAME_BUFFER_HEIGHT( CS_grpGetImageFrameBuffer( img ) );
}


bool SWGameContext::createSound( M_Int32& soundID, const char* filePath, bool isStreaming )
{
    if ( filePath == NULL ) return false;
    std::string fileFullPath = resourcePathBase + filePath;
    SoundTable::iterator itor = soundCacheTable.find( fileFullPath );
    if ( itor != soundCacheTable.end() )
    {
        soundID = (itor->second);
        return true;
    }
    
    M_Int32 result = CS_mdaSoundCreate( &soundID, (M_Char*)fileFullPath.c_str(), 0, 0, isStreaming );
    if ( result != M_E_SUCCESS ) return false;
    result = CS_mdaSoundPrepare( soundID );
    if ( result != M_E_SUCCESS ) return false;
    setSoundVolume( soundID, 100 );
    soundCacheTable.insert( std::make_pair( fileFullPath, soundID ) );
    return true;
}
void SWGameContext::destroySound( M_Int32 soundID )
{
    CS_mdaSoundDestroy( soundID );
}
void SWGameContext::pauseSound( M_Int32 soundID )
{
    CS_mdaSoundPause( soundID );
}
void SWGameContext::resumeSound( M_Int32 soundID )
{
    CS_mdaSoundResume( soundID );
}
void SWGameContext::playSound( M_Int32 soundID, bool isRepeat )
{
    CS_mdaSoundPlay( soundID, isRepeat );
}
void SWGameContext::stopSound( M_Int32 soundID )
{
    CS_mdaSoundStop( soundID );
}
void SWGameContext::rePlaySound( M_Int32 soundID, bool isRepeat )
{
    stopSound( soundID );
    playSound( soundID, isRepeat );
}

void SWGameContext::setSoundVolume( M_Int32 soundID, float volume )
{
    CS_mdaSoundSetVolume( soundID, volume * ( CS_MDA_VOLSEL_MAX - CS_MDA_VOLSEL_MIN ) );
}
float SWGameContext::getSoundVolume( M_Int32 soundID )
{
    M_Int32 volume;
    CS_mdaSoundGetVolume( soundID, &volume );
    return ( (float)volume ) / ( (float)( CS_MDA_VOLSEL_MAX - CS_MDA_VOLSEL_MIN ) );
}

void SWGameContext::setTranslate( float x, float y )
{
    M_Int32 val[2] = { (M_Int32)x, (M_Int32)y };
    CS_grpSetContext( &graphicContext, CS_GRP_CONTEXT_OFFSET_IDX, (void*)val );
}

void SWGameContext::setRotate( float radian )
{
    M_Int32 val = ((M_Int32)(radian*(180.f/3.14f)));
    CS_grpSetContext( &graphicContext, CS_GRP_CONTEXT_ROTATE_IDX, (void*)val );
}

void SWGameContext::setScale( float x, float y )
{
    M_Int32 val[2] = { (M_Int32)(x*100.0f), (M_Int32)(y*100.0f) };
    CS_grpSetContext( &graphicContext, CS_GRP_CONTEXT_SCALE_IDX, (void*)val );
}

void SWGameContext::setAlpha( float alpha )
{
    M_Int32 val = alpha * 255;
    CS_grpSetContext( &graphicContext, CS_GRP_CONTEXT_ALPHA_IDX, (void*)val );
}


void SWGameContext::getTranslate( float& x, float& y )
{
    M_Int32 val[2] = { 0, 0 };
    CS_grpGetContext( &graphicContext, CS_GRP_CONTEXT_OFFSET_IDX, (void*)&val );
    x = (float)val[0];
    y = (float)val[1];
}

void SWGameContext::getRotate( float& radian )
{
    M_Int32 val = 0;
    CS_grpGetContext( &graphicContext, CS_GRP_CONTEXT_ROTATE_IDX, (void*)&val );
    radian = ((float)val)*(3.14f/180.f);
}

void SWGameContext::getScale( float& x, float& y )
{
    M_Int32 val[2] = { 0, 0 };
    CS_grpGetContext( &graphicContext, CS_GRP_CONTEXT_SCALE_IDX, (void*)&val );
    x = ((float)val[0]) / 100.0f;
    y = ((float)val[1]) / 100.0f;
}

void SWGameContext::getAlpha( float& alpha )
{
    M_Int32 val = 0;
    CS_grpGetContext( &graphicContext, CS_GRP_CONTEXT_ALPHA_IDX, (void*)val );
    alpha = ((float)val)/255.0f;
}

void SWGameContext::drawImage( const CS_GrpImage& img, float x, float y, float width, float height )
{
    if ( !img ) return ;
    CS_grpDrawImage( mainFrameBuffer, x, y, width, height, img, 0, 0, &graphicContext );
}

void SWGameContext::drawImageRegion( const CS_GrpImage& img
                                    , float dstX, float dstY
                                    , float dstWidth, float dstHeight
                                    , float srcX, float srcY
                                    , float srcWidth, float srcHeight )
{
    if ( !img ) return ;
    
    float oldScaleX, oldScaleY;
    getScale( oldScaleX, oldScaleY );
    
    float scaleX = ( dstWidth / srcWidth );
    float scaleY = ( dstHeight / srcHeight );
    dstX /= scaleX;
    dstY /= scaleY;
    scaleX = 0.01f + ( oldScaleX * scaleX );
    scaleY = 0.01f + ( oldScaleY * scaleY );
    
    setScale( scaleX, scaleY );
    
    CS_grpDrawImage( mainFrameBuffer, dstX, dstY, srcWidth, srcHeight, img, srcX, srcY, &graphicContext );
    
    setScale( oldScaleY, oldScaleY );
    
}

void SWGameContext::drawFormatedString( float x, float y, int fontSize, const char *format, ... )
{
    static char buf[256];
    
    va_list args;
    va_start( args, format );
    vsprintf( &buf[0], format, args );
    va_end( args );
    
    fontSize = CS_grpGetFont( NULL, (CS_GRP_FT_SIZE_VARIABLE|fontSize), NULL );
    CS_grpSetContext( &graphicContext, CS_GRP_CONTEXT_FONT_IDX, (void*)fontSize );
    CS_grpDrawString( mainFrameBuffer, x, y, &buf[0], strlen( &buf[0] ), &graphicContext );
}

void SWGameContext::drawLine( float x1, float y1, float x2, float y2 )
{
    CS_grpDrawLine( mainFrameBuffer, (int)x1, (int)y1, (int)x2, (int)y2, &graphicContext );
}

void SWGameContext::drawPrimitiveRect( float x, float y, float width, float height )
{
    CS_grpDrawRect( mainFrameBuffer, (int)x, (int)y, width, height, &graphicContext );
}

void SWGameContext::drawPrimitiveCircle( float x, float y, float radius )
{

    float pi = 3.14159265f;
    float step = ( pi / 18.0f ) * 4;
    float radian = 0;
    
    while ( radian < ( 2 * pi ) )
    {
        float x1 = x + radius * cosf( radian );
        float y1 = y + radius * sinf( radian );
        radian += step;
        float x2 = x + radius * cosf( radian );
        float y2 = y + radius * sinf( radian );
        
        drawLine( x1, y1, x2, y2 );
    }

}

void SWGameContext::drawFillRect( float x, float y, float width, float height )
{
    CS_grpFillRect( mainFrameBuffer, (int)x, (int)y, (int)width, (int)height, &graphicContext );
}

void SWGameContext::clearFrame( unsigned char r, unsigned char g, unsigned char b )
{
    int rgb = getRGB();
    setRGB( r, g, b );
    setScale( 1, 1 );
    setRotate( 0 );
    setTranslate( 0, 0 );
    setAlpha( 1 );
    drawFillRect( 0, 0, screenWidth, screenHeight );
    setRGB( rgb );
}
