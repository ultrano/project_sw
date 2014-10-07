//
//  SWTransform.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWTransform.h"
#include "SWGameContext.h"
#include "SWGameScene.h"
#include "SWGameObject.h"
#include "SWLog.h"
#include "SWParam.h"
#include "SWMath.h"
#include "SWObjectStream.h"
#include "SWDefines.h"
#include <algorithm>
#include <math.h>

SWTransform::SWTransform()
	: m_position( 0, 0, 0 )
	, m_rotate()
	, m_euler( 0, 0, 0 )
	, m_scale( 1, 1, 1 )
	, m_needLocalUpdate( true )
	, m_needWorldUpdate( true )
{
}

SWTransform::SWTransform( factory_constructor )
	: m_position( 0, 0, 0 )
	, m_rotate()
	, m_euler( 0, 0, 0 )
	, m_scale( 1, 1, 1 )
	, m_needLocalUpdate( true )
	, m_needWorldUpdate( true )
{

}

SWTransform::~SWTransform()
{
    //SW_OutputLog( "transform", "deleted" );
}

void SWTransform::setParent( SWTransform* parent )
{
	if ( m_parent() == parent ) return;

	SWHardRef<SWGameObject> object = gameObject();

	//! remove from old parent
	{
		SWHardRef<SWGameObject> next = object()->m_next();
		SWHardRef<SWGameObject> prev = object()->m_prev();
		if ( next() ) next()->m_prev = prev();
		if ( prev() ) prev()->m_next = next();
		if ( m_parent() )
		{
			if ( m_parent()->m_child == object() )
			{
				m_parent()->m_child = object();
			}
		} else {
			if ( SW_GC.getScene()->m_rootGO == object() )
			{
				SW_GC.getScene()->m_rootGO = object();
			}
		}
	}

	//! add to new parent
	{
		SWHardRef<SWGameObject> head;
		if ( parent )
		{
			head = parent->m_child();
			parent->m_child = object();
		} else {
			head = SW_GC.getScene()->m_rootGO();
			SW_GC.getScene()->m_rootGO = object();
		}
		object()->m_next = head();
		head()->m_prev = object();
	}

	m_parent = parent;

	if ( m_setParentDelegates.size() == 0 ) return;
	
	SWArray* arr = new SWArray();
	SWHardRef<SWParam> param = new SWParam( arr );
	arr->add( this );
	arr->add( parent );

	m_iterateCopy = m_setParentDelegates;
	SWObject::Array::iterator itor = m_iterateCopy.begin();
	for ( ; itor != m_iterateCopy.end() ;++itor )
	{
		SWDelegator* dg = swrtti_cast<SWDelegator>( (*itor)() );
		if ( dg == NULL )
		{
			SWObject::Array::iterator begin = m_setParentDelegates.begin();
			SWObject::Array::iterator end   = m_setParentDelegates.end();
			SWObject::Array::iterator last = std::remove( begin, end, dg );
			m_setParentDelegates.erase( last, end );
		}
		dg->call( param() );
	}

	param()->destroy();
}

SWTransform* SWTransform::getParent() const
{
	return m_parent();
}

void SWTransform::addSetParentDelegate( SWDelegator* dg )
{
	m_setParentDelegates.push_back( dg );
}

void SWTransform::removeSetParentDelegate( SWDelegator* dg )
{
	if ( !dg ) return;
	removeSetParentDelegate( dg->getObject(), dg->getHandler() );
}

void SWTransform::removeSetParentDelegate( SWObject* object, const SWHandler& handler )
{
	if ( !object ) return;
	if ( !handler.m_method ) return;
	
	SWObject::Array::iterator itor = m_setParentDelegates.begin();
	for ( ; itor != m_setParentDelegates.end() ; ++itor )
	{
		SWDelegator* itorDG = swrtti_cast<SWDelegator>( (*itor)() );
		if ( itorDG->isEqual( object, handler ) )
		{
			SWObject::Array::iterator begin = m_setParentDelegates.begin();
			SWObject::Array::iterator end   = m_setParentDelegates.end();
			SWObject::Array::iterator last = std::remove( begin, end, itorDG );
			m_setParentDelegates.erase( last, end );
			return;
		}
	}
}

const TMatrix4x4& SWTransform::getWorldMatrix()
{
	if ( m_needWorldUpdate )
	{
		m_needWorldUpdate = false;
		updateMatrix();
	}
	return m_worldMat;
}

const TMatrix4x4& SWTransform::getInvWorldMatrix()
{
	if ( m_needWorldUpdate )
	{
		m_needWorldUpdate = false;
		updateMatrix();
	}
	return m_invWorldMat;
}

const TMatrix4x4& SWTransform::getLocalMatrix()
{
	if ( m_needLocalUpdate )
	{
		m_needLocalUpdate = false;
		m_localMat.transform( m_scale, m_rotate, m_position );
	}
	return m_localMat;
}

const TVector3f& SWTransform::getLocalScale() const
{
	return m_scale;
}

const TQuaternion& SWTransform::getLocalRotate() const
{
	return m_rotate;
}

const TVector3f& SWTransform::getLocalEuler() const
{
	return m_euler;
}

const TVector3f& SWTransform::getLocalPosition() const
{
	return m_position;
}

TVector3f SWTransform::getPosition()
{
	const tmat44& worldMat = getWorldMatrix();
	return tvec3( worldMat.m41, worldMat.m42, worldMat.m43 );
}

void SWTransform::setPosition( const tvec3& pos )
{
	setLocalPosition( worldToLocalPoint( pos ) );
}

void  SWTransform::setRotate( const tquat& rotate )
{
	setLocalRotate( worldToLocalRotate( rotate ) );
}

tvec3 SWTransform::worldToLocalPoint( const tvec3& point ) const
{
	if ( SWTransform* parent = getParent() )
	{
		return point* parent->m_invWorldMat;
	}
	return point;
}

tquat SWTransform::worldToLocalRotate( const tquat& rotate ) const
{
	tquat ret = rotate;
	if ( SWTransform* parent = getParent() )
	{
		const tmat44& m = parent->getWorldMatrix();
		float scaleX = tvec3( m.m11, m.m12, m.m13 ).length();
		float scaleY = tvec3( m.m21, m.m22, m.m23 ).length();
		float scaleZ = tvec3( m.m31, m.m32, m.m33 ).length();
		float m11 = m.m11/scaleX;
		float m12 = m.m12/scaleX;
		float m13 = m.m13/scaleX;
		float m21 = m.m21/scaleY;
		float m22 = m.m22/scaleY;
		float m23 = m.m23/scaleY;
		float m31 = m.m31/scaleZ;
		float m32 = m.m32/scaleZ;
		float m33 = m.m33/scaleZ;
		ret.w = sqrt(1.0f + m11 + m22 + m33) / 2.0f;
		float w4 = (4.0f * ret.w);
		ret.x = (m32 - m23) / w4 ;
		ret.y = (m13 - m31) / w4 ;
		ret.z = (m21 - m12) / w4 ;
		
		ret = rotate* ret;
	}
	return ret;
}

void SWTransform::setLocalScale( const TVector3f& scale )
{
	m_scale = scale;
	needUpdateMatrix();
}

void SWTransform::setLocalRotate( const TQuaternion& rotate )
{
	m_rotate = rotate;
	m_rotate.toEuler( m_euler );
	needUpdateMatrix();
}

void SWTransform::setLocalRotate( const tvec3& euler )
{
	m_euler = euler;
	m_rotate.rotate( m_euler );
	needUpdateMatrix();
}

void SWTransform::setLocalPosition( const TVector3f& position )
{
	m_position = position;
	needUpdateMatrix();
}

void SWTransform::move( float stepX, float stepY, float stepZ )
{
	move( tvec3( stepX, stepY, stepZ ) );
}

void SWTransform::move( const tvec3& step )
{
	setLocalPosition( m_position + step );
}

void SWTransform::rotate( float radianX, float radianY, float radianZ )
{
	rotate( tvec3( radianX, radianY, radianZ ) );
}

void SWTransform::rotate( const tvec3& euler )
{
	setLocalRotate( m_euler + euler );
}

SWTransform* SWTransform::find( const tstring& name ) const
{
	const tuint count = name.size();
	if ( count == 0 ) return NULL;

	SWWeakRef<SWTransform> target = this;
	tstring subName;
	tuint offset1 = 0;
	tuint offset2 = 0;

	while ( offset2 < count )
	{
		offset2 += 1;
		if ( name[offset2] == '/' || offset2 == count )
		{
			subName = name.substr( offset1, offset2 - offset1 );
			target = target()->findImmadiate( subName );

			offset1 = offset2 + 1;
		}
		if ( target.isValid() == false ) break;
	}

	return target();
}

SWTransform* SWTransform::findImmadiate( const tstring& name ) const
{
	if ( name.size() == 0 ) return NULL;

	for ( SWGameObject* itor = m_child() ; itor ;  )
	{
		SWGameObject* go = itor;
		itor = itor->m_next();
		if ( go->getName() == name ) return go->getComponent<SWTransform>();
	}
	return NULL;
}

void SWTransform::onAwake()
{
	__super::onAwake();

	//! add to root
	{
		SWHardRef<SWGameObject> hold = gameObject.getRaw();
		SWHardRef<SWGameObject> head = SW_GC.getScene()->m_rootGO();
		if ( head() )
		{
			gameObject()->m_next = head();
			head()->m_prev = gameObject();
		}
		SW_GC.getScene()->m_rootGO = gameObject();
	}

	gameObject()->addUpdateDelegator( GetDelegator( onUpdate ) );
	gameObject()->addFixedRateUpdateDelegator( GetDelegator( onFixedRateUpdate ) );
}

void SWTransform::onRemove()
{
	gameObject()->removeUpdateDelegator( GetDelegator( onUpdate ) );

	for ( SWGameObject* itor = m_child() ; itor ;  )
	{
		SWGameObject* go = itor;
		itor = itor->m_next();
		go->destroyNow();
	}

	//! remove from old parent
	{
		SWHardRef<SWGameObject> object = gameObject();
		SWHardRef<SWGameObject> next = object()->m_next();
		SWHardRef<SWGameObject> prev = object()->m_prev();
		if ( next() ) next()->m_prev = prev();
		if ( prev() ) prev()->m_next = next();
		if ( m_parent() )
		{
			if ( m_parent()->m_child == object() )
			{
				m_parent()->m_child = object();
			}
		} else {
			if ( SW_GC.getScene()->m_rootGO == object() )
			{
				SW_GC.getScene()->m_rootGO = object();
			}
		}
	}
}

void SWTransform::onUpdate()
{
	updateMatrix();

	SWWeakRef<SWTransform> vital = this;
	for ( SWGameObject* go = m_child() ; go ; go = go->m_next() )
	{
		if ( go->isActiveSelf() )
		{
			go->udpate();
			if ( !vital.isValid() ) break;
		}
	}
}

void SWTransform::onFixedRateUpdate()
{
	SWWeakRef<SWTransform> vital = this;
	for ( SWHardRef<SWGameObject> go = m_child()
		; go.isValid() ; go = go()->m_next )
	{
		if ( go()->isActiveSelf() )
		{
			go()->fixedRateUpdate();
			if ( !vital.isValid() ) break;
		}
	}
}

void SWTransform::needUpdateMatrix()
{
	m_needLocalUpdate = true;
	m_needWorldUpdate = true;
}

void SWTransform::updateMatrix()
{
	if ( SWTransform* parent = m_parent() )
	{
		m_worldMat = getLocalMatrix()* parent->getWorldMatrix();
	}
	else
	{
		m_worldMat = getLocalMatrix();
	}
	m_worldMat.inverse( m_invWorldMat );
}

void SWTransform::serialize( SWObjectWriter* writer )
{
	tuint count = 0;
	for ( SWGameObject* go = m_child() ; go ; go = go->m_next() ) count += 1;
	writer->writeUInt( count );
	for ( SWGameObject* go = m_child() ; go ; go = go->m_next() ) writer->writeObject( go );
	
	writer->writeVec3( m_scale );
	writer->writeQuat( m_rotate );
	writer->writeVec3( m_euler );
	writer->writeVec3( m_position );
}

void SWTransform::deserialize( SWObjectReader* reader )
{
	tuint count = reader->readUInt();
	SWGameObject* go = NULL;
	SWGameObject* last = NULL;
	while ( count-- )
	{
		go = (SWGameObject*)reader->readObject();
		go->m_prev = last;
		if ( last ) last->m_next = go;
		else m_child = go;
	}

	reader->readVec3( m_scale );
	reader->readQuat( m_rotate );
	reader->readVec3( m_euler );
	reader->readVec3( m_position );
}
