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
#include "SWRefNode.h"
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
		SWHardRef<SWRefNode> oldNode = object()->m_node();
		oldNode()->ref = NULL;
		object()->m_node = NULL;
		SWRefNode* next = oldNode()->next();
		SWRefNode* prev = oldNode()->prev();
		if ( next ) next->prev = prev;
		if ( prev ) prev->next = next;
		if ( m_parent() )
		{
			if ( m_parent()->m_childNode() == oldNode() )
			{
				m_parent()->m_childNode = next;
			}
		} else {
			if ( SW_GC.getScene()->m_rootNode() == oldNode() )
			{
				SW_GC.getScene()->m_rootNode = next;
			}
		}
	}

	//! add to new parent
	{
		SWHardRef<SWRefNode> newNode = new SWRefNode;
		newNode()->ref = object();
		object()->m_node = newNode();

		SWHardRef<SWRefNode> head;
		if ( parent )
		{
			head = parent->m_childNode();
			parent->m_childNode = newNode();
		} else {
			head = SW_GC.getScene()->m_rootNode();
			SW_GC.getScene()->m_rootNode = newNode();
		}
		newNode()->next = head();
		if ( head() ) head()->prev = newNode();
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

	for ( SWRefNode* node = m_childNode() ; node ; node = node->next() )
	{
		SWGameObject* go = (SWGameObject*)node->ref();
		if ( go && go->getName() == name ) return go->getComponent<SWTransform>();
	}
	return NULL;
}

void SWTransform::onAwake()
{
	__super::onAwake();

	//! add to root
	{
		SWHardRef<SWRefNode> newNode = new SWRefNode;
		newNode()->ref = gameObject.getRaw();
		gameObject()->m_node = newNode();
		SWHardRef<SWRefNode> head = SW_GC.getScene()->m_rootNode();
		if ( head() )
		{
			newNode()->next = head();
			head()->prev = newNode();
		}
		SW_GC.getScene()->m_rootNode = newNode();
	}

	gameObject()->addUpdateDelegator( GetDelegator( onUpdate ) );
	gameObject()->addFixedRateUpdateDelegator( GetDelegator( onFixedRateUpdate ) );
}

void SWTransform::onRemove()
{
	gameObject()->removeUpdateDelegator( GetDelegator( onUpdate ) );

	for ( SWRefNode* node = m_childNode() ; node ; node = node->next() )
	{
		SWGameObject* go = (SWGameObject*)node->ref();
		if ( go ) go->destroyNow();
	}
	m_childNode = NULL;

	//! remove from list
	{
		SWHardRef<SWRefNode> node = gameObject()->m_node();
		if ( node.isValid() )
		{
			node()->ref = NULL;
			SWRefNode* next = node()->next();
			SWRefNode* prev = node()->prev();
			if ( next ) next->prev = prev;
			if ( prev ) prev->next = next;
		}
	}
}

void SWTransform::onUpdate()
{
	updateMatrix();

	SWWeakRef<SWTransform> vital = this;
	for ( SWHardRef<SWRefNode> node = m_childNode() ; node.isValid() ; )
	{
		SWGameObject* go = (SWGameObject*)node()->ref();
		node = node()->next();
		if ( !go ) continue;
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
	for ( SWHardRef<SWRefNode> node = m_childNode() ; node.isValid() ; )
	{
		SWGameObject* go = (SWGameObject*)node()->ref();
		node = node()->next();
		if ( !go ) continue;
		if ( go->isActiveSelf() )
		{
			go->fixedRateUpdate();
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
	for ( SWRefNode* node = m_childNode() ; node ; node = node->next() )
	{
		SWGameObject* go = (SWGameObject*)node->ref();
		if ( go ) count += 1;
	}

	writer->writeUInt( count );
	for ( SWRefNode* node = m_childNode() ; node ; node = node->next() )
	{
		SWGameObject* go = (SWGameObject*)node->ref();
		if ( go ) writer->writeObject( go );
	}
	
	writer->writeVec3( m_scale );
	writer->writeQuat( m_rotate );
	writer->writeVec3( m_euler );
	writer->writeVec3( m_position );
}

void SWTransform::deserialize( SWObjectReader* reader )
{
	tuint count = reader->readUInt();
	SWRefNode* node = NULL;
	SWRefNode* last = NULL;
	while ( count-- )
	{
		SWGameObject* go = (SWGameObject*)reader->readObject();;
		node = new SWRefNode();
		node->prev = last;
		node->ref = go;
		go->m_node = node;
		if ( last ) last->next = node;
		else m_childNode = node;
	}

	reader->readVec3( m_scale );
	reader->readQuat( m_rotate );
	reader->readVec3( m_euler );
	reader->readVec3( m_position );
}
