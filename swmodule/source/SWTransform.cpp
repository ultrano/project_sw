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

	SWGameObject::Ref object = gameObject();

	if ( m_parent() ) m_parent()->m_children.remove( object() );
	else SW_GC.getScene()->m_roots.remove( object() );

	if ( parent ) parent->m_children.push_back( object() );
	else SW_GC.getScene()->m_roots.push_back( object() );

	m_parent = parent;

	if ( m_setParentDelegates.size() == 0 ) return;
	
	SWArray* arr = new SWArray();
	SWHardRef<SWParam> param = new SWParam( arr );
	arr->add( this );
	arr->add( parent );

	SWObject::List copy = m_setParentDelegates;
	SWObject::List::iterator itor = copy.begin();
	for ( ; itor != copy.end() ;++itor )
	{
		SWDelegator* dg = swrtti_cast<SWDelegator>( (*itor)() );
		if ( dg == NULL ) m_setParentDelegates.remove( *itor );
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
	
	SWObject::List::iterator itor = m_setParentDelegates.begin();
	for ( ; itor != m_setParentDelegates.end() ; ++itor )
	{
		SWDelegator* itorDG = swrtti_cast<SWDelegator>( (*itor)() );
		if ( itorDG->isEqual( object, handler ) )
		{
			m_setParentDelegates.remove( *itor );
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
		ret.w = sqrt(1.0 + m11 + m22 + m33) / 2.0;
		float w4 = (4.0 * ret.w);
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

SWTransform* SWTransform::find( const tstring& name )
{
	SWObject::List::iterator itor = m_children.begin();
	for ( ; itor != m_children.end() ;++itor )
	{
		SWGameObject* object = swrtti_cast<SWGameObject>( (*itor)() );
		if ( object->getName() == name ) return object->getComponent<SWTransform>();
	}
	return NULL;
}

void SWTransform::copyChildren( SWObject::List& transList )
{
	transList = m_children;
}

void SWTransform::onAwake()
{
	__super::onAwake();
	SW_GC.getScene()->m_roots.push_back( gameObject.getRaw() );
	gameObject()->addUpdateDelegator( GetDelegator( onUpdate ) );
}

void SWTransform::onRemove()
{
	gameObject()->removeUpdateDelegator( GetDelegator( onUpdate ) );
	SWObject::List copy = m_children;
	SWObject::List::iterator itor = copy.begin();
	for ( ; itor != copy.end() ; ++itor )
	{
		SWGameObject* go = swrtti_cast<SWGameObject>( (*itor)() );
		go->destroyNow();
	}
	copy.clear();
	m_children.clear();
	
	SWGameObject* object = gameObject();
	if ( m_parent() ) m_parent()->m_children.remove( object );
	else SW_GC.getScene()->m_roots.remove( object );
}

void SWTransform::onUpdate()
{
	updateMatrix();
	SWObject::List::iterator itor = m_children.begin();
	for ( ; itor != m_children.end() ;++itor )
	{
		SWGameObject* go = swrtti_cast<SWGameObject>( (*itor)() );
		go->udpate();
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

void SWTransform::serialize( SWObjectWriter* ow )
{
	ow->writeUInt( m_children.size() );
	SWObject::List::iterator itor = m_children.begin();
	for ( ; itor != m_children.end() ; ++itor )
	{
		ow->writeObject( (*itor)() );
	}
	
	ow->writeVec3( m_scale );
	ow->writeQuat( m_rotate );
	ow->writeVec3( m_euler );
	ow->writeVec3( m_position );
}

void SWTransform::deserialize( SWObjectReader* or )
{
	m_children.resize( or->readUInt() );
	for ( int i = 0 ; i < m_children.size() ; ++i )
	{
		m_children.push_back( or->readObject() );
	}

	or->readVec3( m_scale );
	or->readQuat( m_rotate );
	or->readVec3( m_euler );
	or->readVec3( m_position );
}
