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

class SWGameScene;
class SWComponent;

class SWGameObject : public SWObject
{
    SW_RTTI( SWGameObject, SWObject );
	SW_REFERNCE( SWGameObject );
	friend class SWGameScene;

public:
	typedef ttable< tstring,SWObject::Ref > ObjectMap;

private:
    tstring    m_name;
	SWObject::Array m_components;
	SWObject::List  m_addedComponents;

	SWObject::List   m_updateDelegates;
	ObjectMap      m_propTable;

public:

	SWGameObject();
	SWGameObject( const tstring& name );
    ~SWGameObject();

    void awake();
    void destroy();

	void udpate();

	void setName( const tstring& name ) { m_name = name; };
	const tstring&  getName() const { return m_name; };

	template<typename T>
	T* addComponent() { return (T*)addComponent( new T() ); };
	SWComponent* addComponent( SWComponent* comp );

	template<typename T>
	T* getComponent() const { return (T*)getComponent( T::getRtti() ); };
	SWComponent* getComponent( const SWRtti* rtti ) const;
	
	/*
	template<typename T>
	SWObjectList getComponentsOf() { return getComponentsOf( T::getRtti() ); };
	SWObjectList getComponentsOf( const SWRtti* rtti );
	*/

	template<typename T>
	void removeComponent() { removeComponent( T::getRtti() ); };
	void removeComponent( const SWRtti* rtti );

	void removeComponentAll();

	void addUpdateDelegate( SWDelegate* dg );
	void removeUpdateDelegate( SWDelegate* dg );

	void defineProp( const tstring& name );
	bool isDefinedProp( const tstring& name );
	void setProp( const tstring& name, SWObject* value );
	SWObject* getProp( const tstring& name );

	void addPropSetDelegate( const tstring& name, SWDelegate* del );
	void removePropSetDelegate( const tstring& name, SWDelegate* del );
	void removeAllPropSetDelegate( const tstring& name );
	void cleanPropSetDelegate( const tstring& name );

	void sendMessage( const tstring& msgName, SWObject* param );

};

#endif
