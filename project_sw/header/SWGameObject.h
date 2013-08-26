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
#include <map>

//! 전방 선언
class SWGameScene;
class SWComponent;
/**
 @brief 게임 객체. 
 컴포넌트들의 혼용으로 이루어짐.
 각 컴포넌트는 맡은 역할을 하며 상호 작용.
 특별한 경우가 아니면 "SWBehavior"를 상속해 객체를 조작.
 */
class SWGameObject : public SWObject
{
    SW_RTTI( SWGameObject, SWObject );
	friend class SWGameScene;
protected:

	typedef std::map< const SWRtti*, SWHardRef<SWComponent> > ComponentTable;
    std::string m_name;      //< 객체의 이름 (중복 가능)
	ComponentTable m_componentTable;

public:

	SWGameObject();
    ~SWGameObject();

    //! 객체를 깨운다(활성화 시킨다)
    //! 셋팅된 컴포넌트를 Scene에 등록 시키고
    //! SWBehavior 가 장착 되었을시엔 제일 마지막에 SWBehavior::onAwake()도 호출해준다.
    void awake();
    
    //! 컴포넌트 비활성화와 동시에 씬에서 떼어냄
    //! SWBehavior 가 장착 되었을시엔 제일 처음에 SWBehavior::onAsleep()도 호출해준다.
    void destroy();
	void destroyNow();

	void setName( const std::string& name ) { m_name = name; };
	const std::string&  getName() const { return m_name; };

	template<typename T>
	void addComponent() { addComponent( new T() ); };
	void addComponent( SWComponent* comp );

	template<typename T>
	void removeComponent() { removeComponent( T::getType(); ); };
	void removeComponent( const SWRtti* rtti );

	template<typename T>
	T* getComponent() { return (T*)getComponent( T::getRtti() ); };
	SWComponent* getComponent( const SWRtti* rtti );

};

#endif
