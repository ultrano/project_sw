#include "SWRenderer.h"
#include "SWGameObject.h"
#include "SWGameContext.h"
#include "SWGameScene.h"

void SWRenderer::onStart()
{
	SWGameScene* scene = SW_GC.getScene();
	scene->m_renderers.push_back( this );
}

void SWRenderer::onRemove()
{
	SWGameScene* scene = SW_GC.getScene();
	scene->m_renderers.remove( this );
}

void SWRenderer::addPreRenderDelegate( const SWDelegate* del )
{
	if ( !del ) return;
	SWList::iterator itor = m_preRenderDels.begin();
	for ( ; itor != m_preRenderDels.end() ; ++itor )
	{
		SWDelegate* itorDG = swrtti_cast<SWDelegate>( (*itor)() );
		if ( itorDG->isEqual( del ) ) return;
	}
	m_preRenderDels.push_back( del );
}

void SWRenderer::removePreRenderDelegate( const SWDelegate* del )
{
	if ( !del ) return;
	m_preRenderDels.remove( del );
}

void SWRenderer::preRender()
{
	if ( m_preRenderDels.size() == 0 ) return;

	SWObject::Ref vital = this;
	SWList::Value copy = m_preRenderDels;
	SWList::iterator itor = copy.begin();
	for ( ; itor != copy.end() ; ++itor )
	{
		SWDelegate* del = swrtti_cast<SWDelegate>( (*itor)() );
		if ( del ) del->call( this );
		else removePreRenderDelegate( del );
		if ( !vital.isValid() ) return;
	}
}
