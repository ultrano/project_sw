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

class SWGameObject;
class SWComponent;
class SWCollisionManager;

/**
 @brief 게임 플로우의 기본 단위인 scene(장면)을 관리.
 */
class SWGameScene : public SWObject
{
    SW_RTTI( SWGameScene, SWObject );
protected:
    
    typedef std::list< SWHardRef<SWGameObject> > GameObjectList;
    typedef std::list< SWHardRef<SWComponent> >  ComponentList;
    
    //! 객체 ID 유일성을 위한 멤버.
    //! (증가 연산만 사용. 2^32개의 ID를 다 소진 하는건 상식선에서 불가능으로 결론)
    //! (초당 1000개씩 ID가 배당 돼도 다 쓰는대 49일이 걸림)
    unsigned m_maxID;
    
    //! 객체 관리용.
    GameObjectList m_objects;
    GameObjectList m_freeObjects;
    
    //! 컴포넌트 관리용.
    ComponentList m_transforms;
    ComponentList m_drawers;
    ComponentList m_colliders;
    ComponentList m_behaviors;
    ComponentList m_touchSensors;
    
    //! 컴포넌트 업데이트용.
    //! 이터레이트를 도는 중간에 오브젝트의 삭제로 인해
    //! 컴포넌트도 같이 unregister 되는대 이때 이터레이트 사이클이
    //! 붕괴 된다. 그걸 막기 위해 업데이트 전에 사본을 뜨고 그것으로
    //! 이터레이트를 진행. 이외 여러 안전 장치를 시도 해봤지만
    //! 젤루 직관적이고 심플하고 효과적인거 같음.
    //! 컨테이너 복사에 대한 부담에 있지만,
    //! std:: 컨테이너의 경우 capacity 는 유지 되기 때문에
    //! 메모리 할당에 대한 부담은 적어진다.
    //! 컴포넌트는 내부 처리시 객체가 지워졌는지
    //! 레퍼런스 카운트로 사전에 알수 있다.
    //! 레퍼런스 카운팅이 있어 가능한 대처.
    ComponentList m_updates;
    
public:
    
    //! 메소드들
    SWGameScene();
    ~SWGameScene();
    
    //! 비어있는(컴포넌트가 없는) 객체를 생성.
    //! 생성과 함께 씬에 종속 된다.
    //! 객체는 한번 종속된 씬 이외의 씬에는
    //! 종속 될수 없다.
    //! 현재 입양(?) 같은 처리 또한 없다.
    //! 씬이 죽으면 이 메소드로 생성된 객체들 또한 제거 타겟이 된다.
    SWGameObject* createEmptyObject();
    
    //! 객체를 이름으로 찾는다.
    //! 객체의 이름이 중복될 경우 배열의 앞에 있는 한개만 찾는다.
    //! 현재 이름이 등록된 객체만 찾는게 아니라
    //! 소위 대륙횡단써칭(!) 을 하기 때문에 초기화 함수에서만 사용 하길 권장.
    SWGameObject* findAsName( const char* name );
    
    //! 사용이 끝난 객체를 회수.
    //! 관련 컴포넌트는 그대로 놔둔채
    //! 단순히 객체 만(/*중요!*/) 씬에서 분리 시킨다.
    //! SWGameObject::remove 를 사용하길 권장.
    void          removeObject( SWGameObject* object );
    
    //! 컴포넌트 등록 (등록을 해야 컴포넌트로서 활성화가됨)
    void          registerComponent( SWComponent* component );
    
    //! 컴포넌트 척출 
    void          unregisterComponent( SWComponent* component );
    
    void awake();
    void destroy();
    void resume();
    void pause();
    void update( float elapsed );
    void draw();
    void drawDebugInfo();
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
    virtual void onUpdate( float elapsed ) {};
    
    //! @breif 정해진 FPS 마다 화면 갱신시에 호출.
    virtual void onDraw() {};
    virtual void onPostDraw() {};
    
    /*
     @breif 터치 이벤트가 발생하면 호출
     @param type SW_TouchPress, SW_TouchMove, SW_TouchRelease 가 넘어온다.
     @param x 화면상의 가로 위치.
     @param y 화면상의 세로 위치.
     */
    virtual void onHandleTouch( int type, int x, int y ) {};
    
};

#endif
