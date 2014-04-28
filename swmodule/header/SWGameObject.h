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
	SW_REFERENCE( SWGameObject );
	friend class SWGameScene;

public:

	SWGameObject();
	SWGameObject( factory_constructor );
	SWGameObject( const tstring& name );
    ~SWGameObject();

    void destroy();
	void destroyNow();

	void udpate();

	void setName( const thashstr& name ) { m_name = name; }
	const thashstr&  getName() const { return m_name; }

	void setLayerName( const thashstr& layer) { m_layer = layer; }
	const thashstr& getLayerName() const { return m_layer; }

	void setActive( bool active );
	bool isActiveSelf() const;
	bool isActiveInScene() const;

	template<typename T>
	T* addComponent() { return (T*)addComponent( T::getRtti() ); };
	SWComponent* addComponent( const SWRtti* rtti );

	template<typename T>
	T* getComponent() const { return (T*)getComponent( T::getRtti() ); };
	SWComponent* getComponent( const SWRtti* rtti ) const;

	template<typename T>
	void removeComponent() { removeComponent( T::getRtti() ); };
	void removeComponent( const SWRtti* rtti );

	void removeComponentAll();

	void addUpdateDelegator( SWDelegator* dg );
	void removeUpdateDelegator( SWDelegator* dg );

	void defineProp( const tstring& name );
	bool isDefinedProp( const tstring& name );
	void setProp( const tstring& name, SWObject* value );
	SWObject* getProp( const tstring& name );

	void sendMessage( const tstring& msgName, SWObject* param );

protected:

	void serialize( SWObjectWriter* ow );
	void deserialize( SWObjectReader* or );

private:

	void registerComponent( SWComponent* comp );

private:

	typedef ttable< tstring,SWObject::Ref > ObjectMap;
	
	thashstr m_name;
	thashstr m_layer;
	bool     m_active;

	SWObject::Array m_components;
	SWObject::Array m_loadedComponents;
	
	SWObject::List  m_addedComponents;
	SWObject::List  m_updateDelegates;
	SWObject::List  m_updates;

	ObjectMap       m_propTable;
};

#endif
