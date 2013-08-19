//
//  SWGameObject.h
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWGameObject_h
#define prototype_SWGameObject_h

#include "SWObject.h"
#include <string>

//! 전방 선언
class SWGameScene;
class SWTransform;
class SWDrawer;
class SWBehavior;
class SWCollider;
class SWTouchSensor;
/**
 @brief 게임 객체. 
 컴포넌트들의 혼용으로 이루어짐.
 각 컴포넌트는 맡은 역할을 하며 상호 작용.
 특별한 경우가 아니면 "SWBehavior"를 상속해 객체를 조작.
 */
class SWGameObject : public SWObject
{
    SW_RTTI( SWGameObject, SWObject );
protected:
    
    unsigned               m_id;        //< 객체의 유일성을 위한 ID (아직 정형화를 못시켜서 거의 의미 없음)
    std::string            m_name;      //< 객체의 이름 (중복 가능)
    SWWeakRef<SWGameScene> m_scene;     //< 자신이 등록된 Scene
    SWHardRef<SWTransform> m_transform; //< 공간 담당. (위치, 회전, 비율)
    SWHardRef<SWDrawer>    m_drawer;    //< 그리기 담당.
    SWHardRef<SWBehavior>  m_behavior;  //< 행동 양식 담당. (객체 개성 담당)
    SWHardRef<SWCollider>  m_collider;  //< 충돌 및 물리 담당.
    SWHardRef<SWTouchSensor> m_touchSensor; //< 영역이 터치되었는지 판단 담당
    
public:
    
    SWGameObject( unsigned allocId );
    ~SWGameObject();

    //! 객체를 깨운다(활성화 시킨다)
    //! 셋팅된 컴포넌트를 Scene에 등록 시키고
    //! SWBehavior 가 장착 되었을시엔 제일 마지막에 SWBehavior::onAwake()도 호출해준다.
    void awake();
    
    //! 컴포넌트 비활성화와 동시에 씬에서 떼어냄
    //! SWBehavior 가 장착 되었을시엔 제일 처음에 SWBehavior::onAsleep()도 호출해준다.
    void remove();
    
    void setName( const char* name ) { if (name) m_name = name; };
    void setScene( SWGameScene* scene );
    void setTransform( SWTransform* component );
    void setDrawer( SWDrawer* component );
    void setBehavior( SWBehavior* component );
    void setCollider( SWCollider* component );
    void setTouchSensor( SWTouchSensor* component );
    
    unsigned     getId() const { return m_id; };
    const char*  getName() const { return m_name.c_str(); };
    SWGameScene* getScene() const { return m_scene(); };
    SWTransform* getTransform() const { return m_transform(); };
    SWDrawer*    getDrawer() const { return m_drawer(); };
    SWBehavior*  getBehavior() const { return m_behavior(); };
    SWCollider*  getCollider() const { return m_collider(); };
    SWTouchSensor* getTouchSensor() const { return m_touchSensor(); };
};

#endif
