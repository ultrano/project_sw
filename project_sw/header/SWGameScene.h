//
//  SWGameScene.h
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWGameScene_h
#define prototype_SWGameScene_h

#include "SWObject.h"
#include <list>
#include <set>

class SWGameObject;
class SWComponent;
class SWCollisionManager;

/**
 @brief 게임 플로우의 기본 단위인 scene(장면)을 관리.
 */
class SWGameScene : public SWObject
{
    SW_RTTI( SWGameScene, SWObject );
	friend class SWTransform;
	friend class SWRenderer;
protected:

	typedef std::set< SWHardRef<SWGameObject> >  GameObjectSet;
    typedef std::list< SWHardRef<SWGameObject> > GameObjectList;
    typedef std::list< SWHardRef<SWComponent> >  ComponentList;
    
	//! root game objects
	GameObjectList m_roots;
	GameObjectList m_updates;
	ComponentList  m_renderers;
public:
    
    //! 메소드들
    SWGameScene();
    ~SWGameScene();
    
    //! 객체를 찾는다.
    SWGameObject* find( const char* name );
    
    void awake();
    void destroy();
    void resume();
    void pause();
    void update();
    void draw();
    void handleEvent( int type, int x, int y );
    
    //! @breif 처음 시작할때만 한번 호출
    virtual void onAwake() {};
    
    //! @breif 앱이 종료될때 호출
    virtual void onDestroy() {};
    
    //! @breif 앱이 갱신될때 호출
    virtual void onResume() {};
    
    //! @breif 앱이 중지될대 호출
    virtual void onPause() {};
    
    //! @breif 매프레임 갱신시에 호출
    virtual void onUpdate() {};
    
    //! @breif 정해진 FPS 마다 화면 갱신시에 호출.
    virtual void onDraw() {};
    
    /*
     @breif 터치 이벤트가 발생하면 호출
     @param type SW_TouchPress, SW_TouchMove, SW_TouchRelease 가 넘어온다.
     @param x 화면상의 가로 위치.
     @param y 화면상의 세로 위치.
     */
    virtual void onHandleTouch( int type, int x, int y ) {};
    
};

#endif
