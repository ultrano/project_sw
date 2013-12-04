//
//  SWComponent.h
//  prototype
//
//  Created by 한상운 on 13. 5. 27..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWComponent_h
#define prototype_SWComponent_h

#include "SWObject.h"

class SWGameObject;

/**
 @brief 객체에 붙어 다양성을 갖게 해준다.
 */
class SWComponent : public SWObject
{
    SW_RTTI( SWComponent, SWObject );

	friend class SWGameObject;

public:

	SWWeakRef<SWGameObject> gameObject;
    
    SWComponent();
    ~SWComponent();

	template<typename T>
	T* getComponent() { return (T*)getComponent( T::getRtti() ); };
	SWComponent* getComponent( const SWRtti* rtti );

	virtual void onAwake() {};
	virtual void onStart() {};
	virtual void onRemove() {};
};

#endif
