//
//  SWBehavior.h
//  prototype
//
//  Created by 한상운 on 13. 5. 24..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWBehavior_h
#define prototype_SWBehavior_h

#include "SWComponent.h"

class TVector2f;
class SWCollisionInfo;
class SWGameObject;
class SWNotifyMessage;

/** 
 @brief 객체의 행동양식을 담당하는 컴포넌트.
 이 클레스를 상속받아 객체 개성을 유도.
 */
class SWBehavior : public SWComponent
{
    SW_RTTI( SWBehavior, SWComponent );
private:

	typedef ttable< tstring,SWHardRef<SWDelegator> > ReceiverTable;

	ReceiverTable m_recvTable;

public:
    
    SWBehavior();
    ~SWBehavior();
    
    //! 생성이 완료되고 초기에 한번만 호출.
    //! SWGameObject::awake()의 호출시에 같이 호출된다.
    virtual void onAwake() {};
    
    //! 제거 될때 한번만 호출.
    //! SWGameObject::remove()의 호출시에 같이 호출된다.
    virtual void onAsleep() {};
    
    //! 매 프레임 갱신시 호출
    virtual void onUpdate() {};
    
    //! SWCollider 컴포넌트에 의해 호출
    virtual void onCollision( const SWCollisionInfo& info ) {};
    
	void setMessageDelegate( const tstring& msgName, const SWDelegator* del );
	const SWDelegator* getMessageDelegate( const tstring& msgName );
	void receiveMessage( const tstring& msgName, SWObject* param );

private:

	void onStart();
	void onRemove();
	void update( SWGameObject* );
};

#endif
