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
	friend class SWTransform;

public:

	SWGameObject();
	SWGameObject( factory_constructor );
	SWGameObject( const tstring& name );
    ~SWGameObject();

	//! destroy this from scene
	////! GO will be destroyed just before end of frame that calls this.
    void destroy();
	////! GO will be destroyed just in time, but this is not recommended. 
	////! you have to check using SWWeeakRef::isValid() or SWHardRef::isValid()
	void destroyNow();

	void setName( const thashstr& name ) { m_name = name; }
	const thashstr&  getName() const { return m_name; }

	void  setLayer( tuint layer);
	tuint getLayer() const;

	void setActive( bool active );
	bool isActiveSelf() const;
	bool isActiveInScene() const;

	template<typename T>
	T* addComponent() { return (T*)addComponent( T::getRtti() ); };
	SWComponent* addComponent( const SWRtti* rtti );

	template<typename T>
	T* getComponent() const { return (T*)getComponent( T::getRtti() ); };
	SWComponent* getComponent( const SWRtti* rtti ) const;
	SWComponent* getComponent( const tstring& typeName ) const;

	template<typename T>
	void removeComponent() { removeComponent( T::getRtti() ); };
	void removeComponent( const SWRtti* rtti );

	void removeComponentAll();

	void addUpdateDelegator( SWDelegator* dg );
	void removeUpdateDelegator( SWDelegator* dg );
	tuint getUpdateDelegatorCount() const;
	
	void addFixedRateUpdateDelegator( SWDelegator* dg );
	void removeFixedRateUpdateDelegator( SWDelegator* dg );
	tuint getFixedRateUpdateDelegator() const;

	void addLayerDelegator( SWDelegator* dg );
	void removeLayerDelegator( SWDelegator* dg );
	tuint getLayerDelegator() const;

	void sendMessage( const thashstr& msgName, SWObject* param );

protected:

	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

private:

	void registerComponent( SWComponent* comp );
	void udpate();
	void fixedRateUpdate();
	void preUpdate();

private:

	
	thashstr m_name;
	tuint    m_layer;
	bool     m_active;

	SWObject::Array m_components;
	SWObject::Array m_loadedComponents;
	
	SWObject::Array  m_addedComponents;
	SWObject::Array  m_updateDelegates;
	SWObject::Array  m_fixedRateUpdateDelegates;
	SWObject::Array  m_layerDelegates;
	SWObject::Array  m_iterateCopy;
};

#endif
