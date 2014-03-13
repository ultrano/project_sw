#include "SWActDelegate.h"

SWActDelegate::SWActDelegate( SWDelegator* del )
	: m_del( del )
	, m_called( false )
{

}

SWActDelegate::~SWActDelegate()
{

}

bool SWActDelegate::isDone()
{
	return m_called;
}

bool SWActDelegate::onStart()
{
	m_called = true;
	if ( m_del() ) m_del()->call( this );
	return true;
}

void SWActDelegate::onUpdate( float delta )
{

}