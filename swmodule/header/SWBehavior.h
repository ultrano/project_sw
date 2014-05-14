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

public:
    
	SWBehavior();
	SWBehavior( factory_constructor );
    ~SWBehavior();
    
    virtual void onUpdate() {};

	void delegateMessage( const tstring& msgName, SWObject* param );
	void setMessageDelegator( const tstring& msgName, const SWDelegator* del );
	const SWDelegator* getMessageDelegator( const tstring& msgName );

protected:

	void onAwake();
	void onRemove();
	void update();

private:

	typedef ttable< tstring,SWHardRef<SWDelegator> > ReceiverTable;

	ReceiverTable m_recvTable;
};

#endif
