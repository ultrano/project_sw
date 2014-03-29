//
//  SWTransform.h
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWTransform_h
#define prototype_SWTransform_h

#include "SWComponent.h"

/**
 @brief 객체가 공간 개념을 갖게 한다.
        특별한 경우를 제외하고 가장 기본이 되는 컴포넌트
 */
class SWTransform : public SWComponent
{
    SW_RTTI( SWTransform, SWComponent );
private:

	SWWeakRef<SWTransform> m_parent;
	SWObject::List           m_children;
	SWObject::List           m_setParentDelegates;
	
	TVector3f    m_scale;    //< 비율.
	TQuaternion  m_rotate;   //< 회전.
	TVector3f    m_euler;
	TVector3f    m_position; //< 위치.
	TMatrix4x4   m_worldMat; //< world matrix
	TMatrix4x4   m_localMat; //< local matrix
	TMatrix4x4   m_invWorldMat;

	bool m_needLocalUpdate;
	bool m_needWorldUpdate;

	void needUpdateMatrix();

public:
	
	void updateMatrix();
    SWTransform();
    ~SWTransform();

	void setParent( SWTransform* parent );
	SWTransform* getParent() const;

	void addSetParentDelegate( SWDelegator* callBack );
	void removeSetParentDelegate( SWDelegator* callBack );
	void removeSetParentDelegate( SWObject* object, const SWHandler& handler );

	const TMatrix4x4& getWorldMatrix();
	const TMatrix4x4& getLocalMatrix();

	const TVector3f&   getLocalScale() const;
	const TQuaternion& getLocalRotate() const;
	const TVector3f&   getLocalPosition() const;

	void setLocalScale( const tvec3& scale );
	void setLocalRotate( const tquat& rotate );
	void setLocalRotate( const tvec3& euler );
	void setLocalPosition( const tvec3& position );

	tvec3 getPosition();
	void  setPosition( const tvec3& pos );
	void  setRotate( const tquat& rotate );

	tvec3 worldToLocalPoint( const tvec3& point ) const;
	tquat worldToLocalRotate( const tquat& rotate ) const;

	void move( float stepX, float stepY, float stepZ );
	void move( const tvec3& step );
	void rotate( float radianX, float radianY, float radianZ );
	void rotate( const tvec3& euler );

	SWTransform* find( const tstring& name );
	void copyChildren( SWObject::List& transList );

	void onStart();
	void onRemove();
	void onUpdate();

};

#endif
