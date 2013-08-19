//
//  SWGameContext.h
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWGameContext_h
#define prototype_SWGameContext_h

#include "WIPIHeader.h"
#include "SWDefines.h"
#include "SWRefCounter.h"
#include "SWCriticalSection.h"

#include <map>
#include <string>

#define SW_GC ( SWGameContext::getInstance() )

// 전방 선언
class SWGameScene;
class SWImageRegion;
class SWSpriteData;

class SWGameContext
{
    typedef std::map< std::string, CS_GrpImage > ImageTable;
    typedef std::map< std::string, SWHardRef<SWSpriteData> > SpriteTable;
    typedef std::map< std::string, M_Int32 > SoundTable;
public:
    
    // Game Context 의 싱글톤을 얻어온다.
    static SWGameContext& getInstance();
    
    // game world 의 사이즈
    float worldWidth, worldHeight;
    
    // screen 의 사이즈
    int screenWidth, screenHeight;
    
    void setNextScene( SWGameScene* scene );
    
private:
    
    // graphic context
    CS_GrpContext graphicContext;
    
    // main screen frame buffer
    CS_GrpFrameBuffer mainFrameBuffer;
    
    // current scene
    SWHardRef<SWGameScene> currentScene;
    SWHardRef<SWGameScene> nextScene;

    // 업데이트 간격을 알기 위한 변수
    M_Int32 lastFrameTime;
    
    // Frame Per Second (초당 프레임수) 변수
    float fps;
    
    // FPS를 재기 위한 시간 축적 변수 (3초마다 0으로 초기화됨)
    unsigned fpsAccumTime;
    
    // FPS를 재기 위한 프레임 축적 변수 (3초마다 0으로 초기화됨)
    unsigned fpsFrameCount;
    
    // 고정 프레임 드로잉을 위한 변수 (지정한 프레임 마다 화면 갱신을 위한)
    unsigned fixedFrameDrawing;
    
    // CS_APP_EVENT_XXX_TOUCH 이벤트일때 드래그 상대좌표를 절대좌표로 변환하는 용도
    M_Int32 lastTouchedPoint2[2];
    
    int touchEventType;
    int touchedPoint2[2];
    
    // frame loop 를 위한 타이머
    CSTimer loopTimer;
    
    // table for cache
    ImageTable  imageCacheTable;
    SpriteTable spriteCacheTable;
    SoundTable  soundCacheTable;
    
    // onHangleEvent 와 onFrameMove 의 비동기성을 위한 임계영역
    SWCriticalSection m_cs;
    
    //! resource path basement
    std::string resourcePathBase;
    
public:

	void onStart( SWGameScene* firstScene, const std::string& resFolder );
	void onFrameMove();
	void onResume();
	void onPause();
	void onDestroy();
	void onHandleEvent( int type, int param1, int param2 );
    bool onHandleEvent1( int type, int param1, int param2 );
    bool onHandleEvent2( int type, int param1, int param2 );
    bool onHandleEvent3( int type, int param1, int param2 );
    
    void* alloc( size_t size );
    void  free( void* memory );
    
    float getFPS();
    int  getRGB();
    void setRGB( int rgb );
    void setRGB( unsigned char r, unsigned char g, unsigned char b );
    
    void setTranslate( float x, float y );
    void setRotate( float radian );
    void setScale( float x, float y );
    void setAlpha( float alpha );
    
    void getTranslate( float& x, float& y );
    void getRotate( float& radian );
    void getScale( float& x, float& y );
    void getAlpha( float& alpha );
    
    bool createImage( CS_GrpImage* img, const char* filePath );
    void destroyImage( const CS_GrpImage& img );
    int  getImageWidth( const CS_GrpImage& img );
    int  getImageHeight( const CS_GrpImage& img );
    
    bool createSound( M_Int32& soundID, const char* filePath, bool isStreaming );
    void destroySound( M_Int32 soundID );
    void pauseSound( M_Int32 soundID );
    void resumeSound( M_Int32 soundID );
    void playSound( M_Int32 soundID, bool isRepeat );
    void stopSound( M_Int32 soundID );
    void rePlaySound( M_Int32 soundID, bool isRepeat );
    void setSoundVolume( M_Int32 soundID, float volume );
    float getSoundVolume( M_Int32 soundID );
    
    void drawImage( const CS_GrpImage& img, float x, float y, float width, float height );
    void drawImageRegion( const CS_GrpImage& img, float dstX, float dstY, float dstWidth, float dstHeight, float srcX, float srcY, float srcWidth, float srcHeight );
    void drawFormatedString( float x, float y, int fontSize, const char* format, ... );
    void drawLine( float x1, float y1, float x2, float y2 );
    void drawPrimitiveRect( float x, float y, float width, float height );
    void drawPrimitiveCircle( float x, float y, float radius );
    void drawFillRect( float x, float y, float width, float height );
    void clearFrame( unsigned char r, unsigned char g, unsigned char b );
    

};

#endif
