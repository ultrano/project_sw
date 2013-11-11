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
#include <vector>
#include <list>

class SWGameScene;
class SWComponent;

class SWGameObject : public SWObject
{
    SW_RTTI( SWGameObject, SWObject );
	friend class SWGameScene;
protected:

	typedef std::vector< SWHardRef<SWComponent> > ComponentArray;
	typedef std::list< SWHardRef<SWComponent> > ComponentList;

    std::string    m_name;
	ComponentArray m_components;
	ComponentList  m_addedComponents;

	SWObjectList   m_updateDelegates;

public:

	SWGameObject();
	SWGameObject( const std::string& name );
    ~SWGameObject();

    void awake();
    void destroy();

	void udpate();

	void setName( const std::string& name ) { m_name = name; };
	const std::string&  getName() const { return m_name; };

	template<typename T>
	T* addComponent() { return (T*)addComponent( new T() ); };
	SWComponent* addComponent( SWComponent* comp );

	template<typename T>
	T* getComponent() { return (T*)getComponent( T::getRtti() ); };
	SWComponent* getComponent( const SWRtti* rtti );

	template<typename T>
	void removeComponent() { removeComponent( T::getRtti() ); };
	void removeComponent( const SWRtti* rtti );

	void removeComponentAll();

	void addUpdateDelegate( SWDelegate* dg );
	void removeUpdateDelegate( SWDelegate* dg );
};

#endif
